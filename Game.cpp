#include "Game.h"
#include <chrono>
#include <thread>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace std::chrono;

volatile sig_atomic_t Game::resize_flag = 0;

const string STRINGS[3][10] = {
    {"VOID DRIFTER", "1. START", "2. SETTINGS", "3. EXIT", "DIFFICULTY", "LANGUAGE", "SHIP", "BACK", "FAILED", "SCORE"},
    {"VIAJERO VACIO", "1. INICIAR", "2. AJUSTES", "3. SALIR", "DIFICULTAD", "IDIOMA", "NAVE", "ATRAS", "FALLIDO", "PUNTOS"},
    {"VIAJANTE VAZIO", "1. INICIAR", "2. OPCOES", "3. SAIR", "DIFICULDADE", "LINGUA", "NAVE", "VOLTAR", "FALHOU", "PONTOS"}
};

void Game::handle_resize(int sig) {
    resize_flag = 1;
}

Game::Game() : state(MENU), difficulty(EASY), lang(LANG_EN), shipModel('A'), score(0), health(100), frames(0) {
    rng.seed(random_device()());
    signal(SIGWINCH, handle_resize);
    init();
}

void Game::init() {
    term.updateSize();
    player = { {(float)term.width() / 2, (float)term.height() - 2}, {0, 0}, shipModel, 2, true };
    bullets.clear();
    enemies.clear();
    particles.clear();
    stars.clear();
    for(int i=0; i<50; ++i) {
        stars.push_back({(float)(rng() % term.width()), (float)(rng() % term.height())});
    }
    score = 0;
    health = 100;
    frames = 0;
}

string Game::getStr(int index) {
    return STRINGS[lang][index];
}

void Game::run() {
    auto last_time = steady_clock::now();
    while (state != EXIT) {
        auto now = steady_clock::now();
        auto elapsed = duration_cast<milliseconds>(now - last_time).count();

        if (resize_flag) {
            term.updateSize();
            resize_flag = 0;
        }

        if (elapsed >= 16) {
            processInput();
            update();
            render();
            last_time = now;
        } else {
            this_thread::sleep_for(milliseconds(1));
        }
    }
}

void Game::processInput() {
    char c = 0;
    if (Terminal::kbhit()) c = Terminal::getch();

    if (state == MENU) {
        if (c == '1') { init(); state = PLAY; }
        if (c == '2') state = SETTINGS;
        if (c == '3') state = EXIT;
    }
    else if (state == SETTINGS) {
        if (c == '1') {
            if (difficulty == EASY) difficulty = HARD;
            else if (difficulty == HARD) difficulty = NIGHTMARE;
            else difficulty = EASY;
        }
        if (c == '2') {
            if (lang == LANG_EN) lang = LANG_ES;
            else if (lang == LANG_ES) lang = LANG_PT;
            else lang = LANG_EN;
        }
        if (c == '3') {
            if (shipModel == 'A') shipModel = 'V';
            else if (shipModel == 'V') shipModel = 'U';
            else shipModel = 'A';
        }
        if (c == 'b') state = MENU;
    }
    else if (state == GAMEOVER) {
        if (c == 'r') { init(); state = PLAY; }
        if (c == 'm') state = MENU;
    }
    else if (state == PLAY) {
        player.vel = {0, 0};
        if (c == 'w') player.vel.y = -1.0f;
        if (c == 's') player.vel.y = 1.0f;
        if (c == 'a') player.vel.x = -2.0f;
        if (c == 'd') player.vel.x = 2.0f;
        if (c == 'q') state = MENU;
        if (c == ' ') spawnBullet();
    }
}

void Game::spawnBullet() {
    bullets.push_back({ {player.pos.x, player.pos.y - 1}, {0, -1.8f}, '|', 6, true });
}

void Game::spawnEnemy() {
    int chance = (difficulty == EASY) ? 20 : (difficulty == HARD) ? 10 : 5;
    if (frames % chance == 0) {
        float x = rng() % term.width();
        int type = rng() % 3;
        char icon = (type == 0) ? 'X' : (type == 1) ? 'H' : 'W';
        float speed = (0.2f + (rng() % 100) / 300.0f) * (1 + difficulty * 0.5f);
        enemies.push_back({ {x, 0}, {0, speed}, icon, 1, true });
    }
}

void Game::createExplosion(float x, float y) {
    for (int i = 0; i < 6; ++i) {
        float vx = ((rng() % 100) - 50) / 40.0f;
        float vy = ((rng() % 100) - 50) / 40.0f;
        particles.push_back({ {x, y}, {vx, vy}, '*', 3, true });
    }
}

void Game::update() {
    if (state != PLAY) return;

    frames++;

    for (auto& s : stars) {
        s.y += 0.2f;
        if (s.y >= term.height()) {
            s.y = 0;
            s.x = rng() % term.width();
        }
    }

    player.pos.x += player.vel.x;
    player.pos.y += player.vel.y;
    player.pos.x = max(0.0f, min((float)term.width() - 1, player.pos.x));
    player.pos.y = max(0.0f, min((float)term.height() - 1, player.pos.y));

    for (auto& b : bullets) {
        b.pos.y += b.vel.y;
        if (b.pos.y < 0) b.active = false;
    }

    spawnEnemy();

    for (auto& e : enemies) {
        e.pos.y += e.vel.y;
        if (e.icon == 'W') e.pos.x += sin(frames * 0.1f) * 0.5f;

        if (e.pos.y >= term.height()) {
            e.active = false;
        } else if (abs(e.pos.x - player.pos.x) < 2.0f && abs(e.pos.y - player.pos.y) < 1.0f) {
            health -= 20;
            e.active = false;
            createExplosion(e.pos.x, e.pos.y);
        }
    }

    for (auto& p : particles) {
        p.pos.x += p.vel.x;
        p.pos.y += p.vel.y;
        if (p.pos.x < 0 || p.pos.x >= term.width() || p.pos.y < 0 || p.pos.y >= term.height()) p.active = false;
    }

    for (auto& b : bullets) {
        if (!b.active) continue;
        for (auto& e : enemies) {
            if (!e.active) continue;
            if (abs(b.pos.x - e.pos.x) < 2.0f && abs(b.pos.y - e.pos.y) < 1.0f) {
                b.active = false;
                e.active = false;
                score += 100 * (difficulty + 1);
                createExplosion(e.pos.x, e.pos.y);
                break;
            }
        }
    }

    bullets.erase(remove_if(bullets.begin(), bullets.end(), [](const Entity& e){ return !e.active; }), bullets.end());
    enemies.erase(remove_if(enemies.begin(), enemies.end(), [](const Entity& e){ return !e.active; }), enemies.end());
    particles.erase(remove_if(particles.begin(), particles.end(), [](const Entity& e){ return !e.active; }), particles.end());

    if (health <= 0) state = GAMEOVER;
}

void Game::render() {
    string buf;
    buf.reserve(term.width() * term.height() * 12);
    buf += "\033[H";

    if (state == PLAY) {
        vector<string> grid(term.height(), string(term.width(), ' '));
        vector<int> colors(term.height() * term.width(), 0);

        for(auto& s : stars) {
            int sx = (int)s.x; int sy = (int)s.y;
            if(sx >=0 && sx < term.width() && sy >=0 && sy < term.height()) {
                grid[sy][sx] = '.';
                colors[sy * term.width() + sx] = 8;
            }
        }

        auto draw = [&](const Entity& e, int c_code) {
            int ex = (int)e.pos.x; int ey = (int)e.pos.y;
            if(ex >=0 && ex < term.width() && ey >=0 && ey < term.height()) {
                grid[ey][ex] = e.icon;
                colors[ey * term.width() + ex] = c_code;
            }
        };

        for(const auto& p : particles) draw(p, 3);
        for(const auto& b : bullets) draw(b, 6);
        for(const auto& e : enemies) draw(e, 1);
        draw(player, 2);

        for(int y=0; y<term.height(); ++y) {
            for(int x=0; x<term.width(); ++x) {
                int c = colors[y * term.width() + x];
                if (c == 0) buf += " ";
                else if (c == 8) buf += "\033[90m.\033[0m";
                else {
                    buf += "\033[3" + to_string(c) + "m";
                    buf += grid[y][x];
                    buf += "\033[0m";
                }
            }
            if (y < term.height() - 1) buf += "\n";
        }

        string hud = "HP: " + to_string(health) + " | " + getStr(9) + ": " + to_string(score);
        buf += "\033[0;0H\033[44m\033[37m" + hud + "\033[0m";
    }
    else {
        for(int y=0; y<term.height(); ++y) buf += string(term.width(), ' ') + "\n";
        int cx = term.width() / 2;
        int cy = term.height() / 2;

        auto center = [&](int y_off, string s, string color = "") {
            buf += "\033[" + to_string(cy + y_off) + ";" + to_string(cx - s.length()/2) + "H" + color + s + "\033[0m";
        };

        if (state == MENU) {
            center(-3, getStr(0), "\033[36m");
            center(0, getStr(1));
            center(1, getStr(2));
            center(2, getStr(3));
        }
        else if (state == SETTINGS) {
            string dTxt = (difficulty==EASY) ? "EASY" : (difficulty==HARD) ? "HARD" : "NIGHTMARE";
            string lTxt = (lang==LANG_EN) ? "ENGLISH" : (lang==LANG_ES) ? "ESPANOL" : "PORTUGUES";
            string sTxt = string(1, shipModel);

            center(-3, getStr(2), "\033[36m");
            center(-1, "[1] " + getStr(4) + ": " + dTxt);
            center(1, "[2] " + getStr(5) + ": " + lTxt);
            center(3, "[3] " + getStr(6) + ": " + sTxt);
            center(6, "[b] " + getStr(7));
        }
        else if (state == GAMEOVER) {
            center(-2, getStr(8), "\033[31m");
            center(0, getStr(9) + ": " + to_string(score));
            center(2, "[r] RESTART   [m] MENU");
        }
    }
    cout << buf << flush;
}