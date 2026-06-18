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
#include <filesystem>

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
          std::unordered_set<node*> visited;
        
        public:
         //Insere um novo nó no grafo com o rótulo s
          void insert_nodo(const std::string& prb_id,
                            const std::string& hop, 
                            const std::string& probe_src, 
                            const std::string& dst_addr,
                            const std::string& hop_from,
                            const std::string& rtt){
            if(map[prb_id].nodes.count(hop) == 0){
                map[prb_id].nodes[hop] = {probe_src, dst_addr, rtt, hop_from};
            }
          }

          void exibe_grafo(){
            for (auto& [prb_id, digrafo] : map) {
            std::cout << "prb_id: " << prb_id << "\n";
                for (const auto& [hop, node] : digrafo.nodes) {
                    std::cout << "    hop: " << hop << "\n";
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

          //retorna a quantidade de nós (vertices) do grafo
          size_t size(){
            return nodes.size();
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
            for(auto& [prb_id, digrafo] : map){
                if(digrafo.nodes.count(hop_from) == 0){
                    for(auto& [hop, node] : digrafo.nodes){
                        if(node.hop_from == hop_from){
                            // Verifica duplicata antes de inserir
                            auto it = find(node.links.begin(), node.links.end(), hop_to);
                            if(it == node.links.end()){  // ou usa existe_aresta
                                node.links.push_back(hop_to);
                            }
                            return true;
                        }
                    }
                }
            }
            return false;
          }
          
          //existe uma aresta de?
          bool existe_aresta(const std::string& vertice, const std::string& novoLink){
            auto p = find_node(vertice);
            for (auto& link : p->links){
                if(link == novoLink){
                    return true;
                }
            }
            return false;
          }
          /*
          //numero de arestas que saem de um vertice
          size_t outdegree(const std::string& s){
            for(auto node : nodes){
              if(node.first == s){
                return node.second.links.size();
              }
            }
            return 0;
          }

          //numero de arestas que chegam a um vertice
          size_t indegree(const std::string& s){
            auto p = find(s);
            if(p == nullptr){
              return 0;
            }
            size_t qnt = 0;
            for(auto vertice : nodes){
                for(auto link : vertice.second.links)
                  if(link == s){
                    qnt++;
                  }
            }
            return qnt;
          }
          
          //numero de arestas conectadas a um vertice
          size_t degree(const std::string& s){
            auto p = find(s);
            if(p == nullptr){
              return 0;
            }
            return indegree(s) + outdegree(s);
          }
          
          */
         


          void export2dot(const std::string& filename){
            std::ofstream dot(filename); // cria o arquivo
              dot << "digraph {\n";
                    for (auto& [prb_id, digrafo] : map) {
                        for (auto& [hop, node] : digrafo.nodes) {
                            if (node.links.empty())
                                continue;
                            dot << "\"" << node.hop_from << "\" -> {";
                            for (auto& link : node.links) {
                                dot << "\"" << link << "\" ";
                            }
                            dot << "};\n";
                        }
                    }
                    dot << "}\n";
            dot.close();
            };
          
          
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
            export2dot("graphED2.dot");
            std::string command = "dot -Tpdf graphED2.dot -o " + input + ".pdf";
            std::system(command.c_str());
          }

          void drawMenor_caminhoScreen(std::vector<std::string> path, std::string input){
            export2dot("graphED2.dot");
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

              
                    for (auto& [prb_id, digrafo] : map) {
                        for (auto& [hop, node] : digrafo.nodes) {
                            if (node.links.empty())
                                continue;
                            dot << "\"" << node.hop_from << "\" -> {";
                            for (auto& link : node.links) {
                                dot << "\"" << link << "\" ";
                            }
                            dot << "};\n";
                        }
                    }
                    dot << "}\n";
            dot.close();
            };

        /*
          
          void remove_link(const std::string &from, const std::string &to){
            auto pfrom = find(from);
            if(!pfrom) return;
            auto pto = find(to);
            if(!pto) return;   
            
            auto it = std::find(pfrom->links.begin(), pfrom->links.end(), pto);
            if(it == pfrom->links.end()) return;
            pfrom->links.erase(it);
          }

          void remove_nodo(const std::string &key){
            //encontrar se aquele nodo é vizinho de algum outro nodo e remover
            auto p = find(key);
            if(!p) return;
            for(auto &[k,nd] : nodes){
              auto it = std::find(nd.links.begin(), nd.links.end(), p);
              if(it != nd.links.end()) nd.links.erase(it);
            }
            nodes.erase(key);
          }

          void recursive_DFS(node* p, int level = 0){
            if(visited.count(p) > 0)//ja foi visitado
            return;
            std::cout << std::string(level, '\t') << p->value << std::endl;
            visited.insert(p);
            for(auto ln : p->links){
              recursive_DFS(ln, level+1);
            }
          } 

          void DFS_from(const std::string &s){
            auto p = find(s);
            if(!p) return;
            visited.clear();
            recursive_DFS(p);
          }
        */
          
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