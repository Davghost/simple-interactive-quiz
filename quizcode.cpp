#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>
using namespace std;

struct questao {
    string pergunta;
    string alternativa[4];
    char resposta;
};

struct Jogador {
    string nome;
    int pontos;
};

char lerRespostaUsuario() {
    char resposta_usuario;
    while (true) {
        cout << "Digite sua resposta (A-D): ";
        cin >> resposta_usuario;
        resposta_usuario = toupper(resposta_usuario);
        if (resposta_usuario >= 'A' && resposta_usuario <= 'D') {
            return resposta_usuario;
        } else {
            cout << "Opcao invalida. Digite novamente (A-D):" << endl;
        }
    }
}

int main() {
    ifstream arquivo("perguntas.txt");
    if (!arquivo.is_open()) {
        cout << "ERRO: Arquivo 'perguntas.txt' não encontrado ou inválido." << endl;
        return 1;
    }

    questao questões[3];
    string pergunta, alter1, alter2, alter3, alter4, lixo;
    int aux = 0;
    char resposta;

    for (int i = 0; i < 3; i++) {
        getline(arquivo, pergunta);
        getline(arquivo, alter1);
        getline(arquivo, alter2);
        getline(arquivo, alter3);
        getline(arquivo, alter4);
        arquivo >> resposta;
        arquivo.ignore();
        getline(arquivo, lixo);

        questões[i].pergunta = pergunta;
        questões[i].alternativa[0] = alter1;
        questões[i].alternativa[1] = alter2;
        questões[i].alternativa[2] = alter3;
        questões[i].alternativa[3] = alter4;
        questões[i].resposta = resposta;
    }
    arquivo.close();

    for (int i = 0; i < 3; i++) {
        cout << "Pergunta " << i + 1 << ": " << questões[i].pergunta << endl;
        cout << questões[i].alternativa[0] << endl;
        cout << questões[i].alternativa[1] << endl;
        cout << questões[i].alternativa[2] << endl;
        cout << questões[i].alternativa[3] << endl;

        char resposta_usuario = lerRespostaUsuario();

        if (toupper(resposta_usuario) == toupper(questões[i].resposta)) {
            cout << "Acertou!" << endl;
            aux++;
        } else {
            cout << "Errou!" << endl;
        }
        cout << endl;
    }

    cout << "Quiz Finalizado" << endl;
    cout << "Você acertou " << aux << " de 3 perguntas!" << endl;
    cout << "Digite seu primeiro nome: ";

    string nome;
    cin.ignore();
    getline(cin, nome);

    ifstream rankingfile("ranking.txt");
    vector<Jogador> ranking;
    if (rankingfile) {
        string nome_arquivo;
        int pontos_arquivo;
        while (rankingfile >> nome_arquivo >> pontos_arquivo) {
            Jogador j;
            j.nome = nome_arquivo;
            j.pontos = pontos_arquivo;
            ranking.push_back(j);
        }
        rankingfile.close();
    }

    bool encontrou = false;
    for (auto& j : ranking) {
        if (j.nome == nome) {
            encontrou = true;
            if (aux > j.pontos) {
                // Atualiza apenas se a nova pontuação for maior
                j.pontos = aux;
            } else {
                // Não atualiza, pois a pontuação é menor ou igual
            }
            break;
        }
    }

    if (!encontrou) {
        Jogador novo;
        novo.nome = nome;
        novo.pontos = aux;
        ranking.push_back(novo);
    }

    ofstream rankingfile_out("ranking.txt");
    if (!rankingfile_out) {
        cout << "ERRO: Não foi possível salvar o ranking." << endl;
        return 1;
    }

    for (const auto& j : ranking) {
        rankingfile_out << j.nome << " " << j.pontos << endl;
    }
    rankingfile_out.close();

    sort(ranking.begin(), ranking.end(), [](Jogador a, Jogador b) {
        return a.pontos > b.pontos;
    });

    cout << "==== RANKING ====" << endl;
    for (const auto& j : ranking) {
        cout << j.nome << " " << j.pontos << endl;
    }

    return 0;
}