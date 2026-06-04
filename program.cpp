#include <iostream>
#include <fstream>
#include <sstream>
#include "graph.cpp"
#include <unordered_map>

using namespace std;

int main(int argc, char *argv[]){
    string prb_id, probe_src, dst_addr, rtt, hop_from, hop_to, hop;
    ifstream arquivo_entrada;
    string entrada;
    graph::digraph g;
    unordered_map<string, graph::digraph> map;
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
                    map[prb_id].nodes[hop] = {probe_src, dst_addr, rtt, hop_from};
                    map[prb_id].nodes[hop].links.push_back();
                }
            }
        }
   }
   

   for (auto& [prb_id, digrafo] : map) {
    std::cout << "prb_id: " << prb_id << "\n";
        for (const auto& [hop, node] : digrafo.nodes) {
            std::cout << "  hop: " << hop << "\n";
            std::cout << "    probe_src: " << node.probe_src << "\n";
            std::cout << "    dst_addr: "  << node.dst_addr  << "\n";
            std::cout << "    rtt: "       << node.rtt       << "\n";
            std::cout << "    hop_from: "  << node.hop_from  << "\n";
            std::cout << "    hop_to:";
            for (auto& destino : node.links) {
                std::cout << " " << destino;
            }
            std::cout << "\n";
        }
    }
    cout << g.size() << " nodos" << endl;
    cout << g.find("192.168.3.1");
    arquivo_entrada.close();
    return 0;
}