/// biblioteca para grafos
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <unordered_set>
#include <queue>

namespace graph{

  class digraph; // declaração antecipada da classe digraph
  extern std::unordered_map<std::string, digraph> map; 

    class digraph{
        private:
          // nodos do grafo - VERTÍCES
          struct node{
            //std::string prb_id;
            //std::string hop;
            std::string probe_src;
            std::string dst_addr;
            std::string rtt;
            std::string hop_from;
            std::vector<std::string> links; //lista de ponteiros p/ os nodos vizinhos - ARESTAS
          };

          //tabela hash, que mapeia o rótulo do nó para o objeto node
                             //<chave>, <valor>    
          std::unordered_map<std::string,node> nodes;

        public:
         //Insere um novo nó no grafo com o rótulo s
          void insert_nodo(const std::string& prb_id,
                            const std::string& hop, 
                            const std::string& probe_src, 
                            const std::string& dst_addr,
                            const std::string& hop_from,
                            const std::string& rtt){
            if(hop_from != "hop_from"){
              if(find_node(hop_from) == nullptr){
                  map[prb_id].nodes[hop_from] = {probe_src, dst_addr, rtt, hop_from};
              }
            }
          }

          void exibe_grafo(){
            for (auto& [prb_id, digrafo] : map) {
            std::cout << "prb_id: " << prb_id << "\n";
                for (const auto& [hop, node] : digrafo.nodes) {
                    std::cout << "    hop_from: " << node.hop_from << "\n";
                    std::cout << "    probe_src: " << node.probe_src << "\n";
                    std::cout << "    dst_addr: "  << node.dst_addr  << "\n";
                    std::cout << "    rtt: "       << node.rtt       << "\n";
                    std::cout << "    links:";
                    for (auto& destino : node.links) {
                        std::cout << " " << destino;
                    }
                    std::cout << "\n";
                }
            }
          }

          //busca um nó pelo seu rótulo e retorna o endereço do nodo
          node* find_node(const std::string& s){
            for(auto& [prb_id, digrafo] : map){
                for(auto& [hop, node] : digrafo.nodes){
                    if(node.hop_from == s){
                        return &node;
                    }
                }
            }

            return nullptr;
          }

          
          //INsere um aresta dirigida de 'from' para 'to'
          bool insert_link(const std::string& hop_from, const std::string& hop_to){
            auto p = find_node(hop_from);
            if(p == nullptr){
              return false;
            }
            // Verifica duplicata antes de inserir
            auto it = find(p->links.begin(), p->links.end(), hop_to);
            if(it == p->links.end()){
                p->links.push_back(hop_to);
            }
            return true;
          }
          
          //numero de arestas que chegam a um vertice
          size_t indegree(const std::string& s){
            auto p = find_node(s);
            size_t qnt = 0;
            if(p == nullptr){
              return 0;
            }
            for(auto& [prb_id, digrafo] : map){
              for(auto& [hop, node] : digrafo.nodes){
                for(auto& link : node.links){
                  if(s == link){
                    qnt++;
                  }
                }
              }
            }
            return qnt;
          }

          void top_criticos(){
            struct nos
            {
              std::string ip;
              int arestas_entradas;
            };

            std::vector<nos> v;
            for(auto& [prb_id, digrafo] : map){
              for(auto& [hop, nodo] : digrafo.nodes){
                  v.push_back({nodo.hop_from, (int)indegree(nodo.hop_from)});
              }
            }
            std::sort(v.begin(), v.end(), [](const nos& a, const nos& b){
              return a.arestas_entradas > b.arestas_entradas;
            });

            std::cout << "Top 5 Roteadores Críticos (maior grau de entrada):\n";
            size_t limite = std::min(v.size(), (size_t)5);
            for(size_t i = 0; i < limite; ++i){
              std::cout << i + 1 << ". " << v[i].ip << " - indegree: " << v[i].arestas_entradas << "\n";
            }
          }
          
          void export2dot(const std::string& filename){
              std::ofstream dot(filename);
              dot << "digraph {\n";
              
              std::unordered_set<std::string> arestas_escritas;
              
              for (auto& [prb_id, digrafo] : map) {
                  for (auto& [hop, node] : digrafo.nodes) {
                      for (auto& link : node.links) {
                          std::string aresta = node.hop_from + "->" + link;
                          if(arestas_escritas.count(aresta) == 0){
                              dot << "\"" << node.hop_from << "\" -> \"" << link << "\";\n";
                              arestas_escritas.insert(aresta);
                          }
                      }
                  }
              }
              dot << "}\n";
              dot.close();
          }
          
          
          void draWhithScreen(const std::string& input){
            export2dot("graphED2.dot");
            std::system("dot -Tx11 graphED2.dot");
          }

          void drawPNG(const std::string& input){
            export2dot("graphED2.dot");
            std::string command = "dot -Tpng graphED2.dot -o " + input + ".png";
            std::system(command.c_str());
          }

          void drawPDF(const std::string& input){
            export2dot("graphED2.dot");
            std::string command = "dot -Tpdf graphED2.dot -o " + input + ".pdf";
            std::system(command.c_str());
          }

          void drawMenor_caminhoPNG(std::vector<std::string> path, std::string input){
            export2dot_pintado("graphED2.dot", path);
            std::string command = "dot -Tpng graphED2.dot -o " + input + ".png";
            std::system(command.c_str());
          }

          void drawMenor_caminhoPDF(std::vector<std::string> path, std::string input){
            export2dot_pintado("graphED2.dot", path);
            std::string command = "dot -Tpdf graphED2.dot -o " + input + ".pdf";
            std::system(command.c_str());
          }

          void drawMenor_caminhoScreen(std::vector<std::string> path, std::string input){
            export2dot_pintado("graphED2.dot", path);
            std::system("dot -Tx11 graphED2.dot");
          }

          void export2dot_pintado(const std::string& filename, std::vector<std::string> path){
            std::ofstream dot(filename); // cria o arquivo
            dot << "digraph {\n";
              for(auto s : path){
                for(auto&[prb_id, digrafo] : map){
                  for(auto& [hop, node] : digrafo.nodes){
                    if(node.hop_from == s){
                      dot << "\t\"" << node.hop_from << "\" [style=filled, fillcolor=\"lightblue\"];\n";
                    }
                  }
                }
              }
              std::unordered_set<std::string> arestas_escritas;
              
                for (auto& [prb_id, digrafo] : map) {
                  for (auto& [hop, node] : digrafo.nodes) {
                      for (auto& link : node.links) {
                          std::string aresta = node.hop_from + "->" + link;
                          if(arestas_escritas.count(aresta) == 0){
                              dot << "\"" << node.hop_from << "\" -> \"" << link << "\";\n";
                              arestas_escritas.insert(aresta);
                          }
                      }
                  }
              }
              dot << "}\n";
            }

        std::vector<std::string> shortest_path(const std::string &from,
                                                const std::string &to)
        {
          std::vector<std::string> path;
          auto pfrom = find_node(from);
          if (!pfrom)
            return path;

          auto pto = find_node(to);
          if (!pto)
            return path;

          if (pfrom == pto)
          {
            path.push_back(from);
            return path;
          }
          std::unordered_map<node*, node*> source;
          std::queue<node*> q;
          q.push(pfrom);
          source[pfrom] = nullptr;
          bool found = false;

          while (!q.empty())
          {
            auto current = q.front();
            q.pop();
            if (current == pto)
            {
              found = true;
              break;
            }
            for (auto& vizinho_id : current->links) {
              auto p = find_node(vizinho_id);
              if (p != nullptr && source.count(p) == 0) {
                  q.push(p);
                  source[p] = current;
                }
            }
          }
          if (found)
          {
            auto p = pto;
            while (p != nullptr)
            {
              path.push_back(p->hop_from);
              p = source[p];
            }
            std::reverse(path.begin(), path.end());
          }
          return path;
        }


        int diametro() {
              int max_diametro = 0;

              for (auto& [prb_id, digrafo] : map) {
                  for (auto& [hop, n] : digrafo.nodes) {
                      std::unordered_map<node*, int> distancias;
                      std::queue<node*> fila;
                      node* start = &n;
                      
                      fila.push(start);
                      distancias[start] = 0;

                      while (!fila.empty()) {
                          node* current = fila.front();
                          fila.pop();
                          int dist_atual = distancias[current];
                          
                          if (dist_atual > max_diametro) {
                              max_diametro = dist_atual;
                          }

                          for (const std::string& vizinho_id : current->links) {
                              node* vizinho = find_node(vizinho_id);
                              if (vizinho && distancias.find(vizinho) == distancias.end()) {
                                  distancias[vizinho] = dist_atual + 1;
                                  fila.push(vizinho);
                              }
                          }
                      }
                  }
              }
              return max_diametro;
          }
    }; /// fim da classe digraph
} //fim do namespace 