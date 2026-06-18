#include <iostream>
#include <fstream>
#include <sstream>
#include "graph.cpp"
#include <unordered_map>
#include <vector>
#include <queue>

using namespace std;

std::unordered_map<std::string, graph::digraph> graph::map;

int main(int argc, char *argv[]){
    string prb_id, probe_src, dst_addr, rtt, hop_from, hop_to, hop;
    ifstream arquivo_entrada;
    string entrada;
    graph::digraph g;
    int arestas = 0;
    int vertices = 0;
    for (int i = 1; i < argc; ++i) {
        entrada += argv[i];
    }

    arquivo_entrada.open(entrada);

   if(!arquivo_entrada){
        cout << "Não foi possível abrir o arquivo ou o nome do arquivo não foi fornecido" << endl;
        exit(1);
   }

   string linha;
   while (getline(arquivo_entrada, linha))
   {

        stringstream ss(linha);
        if(getline(ss, prb_id, ',') 
           && getline(ss, probe_src, ',') 
           && getline(ss, dst_addr, ',') 
           && getline(ss, hop, ',') 
           && getline(ss, hop_from, ',') 
           && getline(ss, hop_to, ',') 
           && getline(ss, rtt, ',')){
            
             
            if(hop_to != "*"){
                if(hop_from != " " || hop_to != " "){
                    g.insert_nodo(prb_id, hop,probe_src, dst_addr, hop_from, rtt);
                    vertices++;
            
                    g.insert_link(hop_from, hop_to);
                    arestas++;
                    
                
                }
            }
        }
   }  
    cout << "Carregamento inicial concluído! Foram inseridos " << vertices << " vertices e " << arestas << " arestas" << endl;
    arquivo_entrada.close();
    cout << "======MENU======" << endl <<
    "Digite a opção desejada:" << endl << 
    "1: Exibir Grafo" << endl <<
    "2: Encontrar o Menor Caminho" << endl <<
    "3: Calcular o Diâmetro do Grafo" << endl <<
    "4: Identificar Roteadores Críticos" << endl <<
    "5: Sair" << endl <<
    "================" << endl <<
    "Escolha uma das opções acima: ";
    int usuario;
    cin >> usuario;

    if(usuario == 1){
        cout << "Digite qual como quer vizualizar o grafo: " << endl <<
        "1: Tela" << endl <<
        "2: Imagem (PNG)" << endl <<
        "3: Documento (PDF)" << endl;
        int escolha;
        cin >> escolha;
        ofstream dot("graphED2.dot"); // cria o arquivo        
        if(escolha == 1){
            g.draWhithScreen(argv[1]);
        }
        else if(escolha == 2){
            g.drawPNG(argv[1]);
        }
        else if(escolha == 3){
            g.drawPDF(argv[1]);
        }
        
        
    }else if (usuario == 2){
        cout << "Digite o IP de Origem:" << endl;
        string ipOrigem;
        cin >> ipOrigem;
        cout << "Digite o IP de Destino:" << endl;
        string ipDestino;
        cin >> ipDestino;
        auto path = g.shortest_path(ipOrigem, ipDestino);
        cout << "Caminho encotrado (" << path.size() - 1 << "):";
        for(auto n : path){
            cout << n << " -> "; 
        }
        cout << endl;
        cout << "Digite qual como quer vizualizar o grafo: " << endl <<
        "1: Tela" << endl <<
        "2: Imagem (PNG)" << endl <<
        "3: Documento (PDF)" << endl;
        int imagem;
        cin >> imagem;
        if(imagem == 1){
            g.drawMenor_caminhoScreen(path,argv[1]);
        }else if(imagem == 2){
            g.drawMenor_caminhoPNG(path,argv[1]);
        }else if(imagem == 3){
            g.drawMenor_caminhoPDF(path,argv[1]);
        }
    }
    else if(usuario == 3){
        cout << g.diametro() << endl;
    }
    else if(usuario == 4){
        
    }
    else if(usuario == 5){
        cout << "Saindo do programa..." << endl;
        return 0;
    }else{
        cout << "Opção inválida. Saindo do programa..." << endl;
        return 0;
    }
    
    
    return 0;
};