Abordagens face às Análises de Risco



Análise de Risco para K = 1
1. Abordagem
Para identificar revisores críticos (pontos únicos de falha), avaliamos a robustez da nossa rede de fluxo simulando a ausência de cada revisor individualmente. A abordagem consiste nos seguintes passos:
1.	Iterar pela lista de todos os revisores disponíveis.
2.	Para cada revisor, "desativar" temporariamente o seu vértice correspondente no grafo (na nossa implementação, conseguimos isto de forma eficiente instruindo o algoritmo BFS a ignorar este nó específico,
   evitando o custo computacional de remover e voltar a adicionar arestas fisicamente).
3.	Executar o algoritmo de Edmonds-Karp nesta rede modificada para calcular o novo fluxo máximo.
4.	Comparar o fluxo máximo resultante com o número total de revisões necessárias (NumSubmissoes $\times$ MinReviews). Se o fluxo máximo for inferior a este limite, o revisor desativado é assinalado como um "risco crítico".
5.	Restaurar o nó do revisor e avançar para o próximo.
   
2. Discussão da Complexidade
O algoritmo de Edmonds-Karp tem uma complexidade temporal de O(V x E^2), onde V é o número de vértices e E é o número de arestas.
Nesta variante, executamos o algoritmo de fluxo máximo exatamente R vezes, onde R é o número total de revisores. Portanto, a complexidade temporal global desta análise de risco é O(R x V x E^2).
Como R <= V, no pior cenário absoluto, a complexidade é limitada por O(V^2 x E^2). Esta complexidade é perfeitamente viável para a verificação de um único revisor em falta (K=1).

3. Exemplo de Input Não-Trivial e Output
•	Cenário de Input: * 3 Submissões (S1, S2, S3), necessitando de 1 revisão cada. (Fluxo total necessário = 3).
  o	3 Revisores (R1, R2, R3).
  o	Capacidades: R1 pode fazer 2 revisões. R2 e R3 só podem fazer 1 revisão cada.
  o	Ligações (Matches possíveis):
  	S1 está ligada a R1 e R2.
  	S2 está ligada a R1 e R3.
  	S3 está ligada a R2 e R3.
•	Execução: Se removermos o R1, os revisores restantes são R2 (máx 1) e R3 (máx 1). Embora S1, S2 e S3 continuem a ter arestas que as ligam a revisores disponíveis, a capacidade absoluta total da rede restante cai para $1 + 1 = 2$.
•	Output: O sistema precisa de 3 revisões, mas apenas consegue fornecer 2. A atribuição é impossível. O algoritmo devolve corretamente o R1 como um revisor crítico.
________________________________________

Formulação Básica com Análise de Risco = K (K > 1)

1. Esboço da Abordagem
Para analisar o risco de $K$ revisores falharem em simultâneo, não podemos simplesmente testar os revisores um a um. Precisamos de combinar um Algoritmo de Geração de Combinações com a nossa base de Fluxo Máximo. A abordagem conceptual é:
    1.	Utilizar um algoritmo para gerar todas as combinações possíveis de tamanho $K$ a partir do conjunto total de $N$ revisores.
    2.	Para cada combinação (grupo) gerada, desativar temporariamente todos os $K$ nós na rede de fluxo em simultâneo.
    3.	Executar o algoritmo de Edmonds-Karp para calcular o novo fluxo máximo.
    4.	Se o fluxo máximo for inferior ao total de revisões exigidas, assinalar esse grupo específico de $K$ revisores como um "grupo de risco".
    5.	Restaurar os $K$ nós e avançar para a próxima combinação.


2. Discussão da Complexidade
O número de formas de escolher K revisores de um total de N é dado pelo coeficiente binomial N!/(K!(N-K)!).
Para cada uma destas combinações, temos de executar o algoritmo de Edmonds-Karp, que demora O(V x E^2).
Assim, a complexidade temporal teórica total é $O(N!/(K!(N-K)!) x V x E^2).
Como N!/(K!(N-K)!) se comporta como O(N^K) para K pequenos, a complexidade cresce de forma polinomial em relação ao número de revisores, mas de forma exponencial em relação a K.
Para um dataset real com centenas de revisores, mesmo um K=3 resultaria em milhões de execuções do Edmonds-Karp, tornando esta abordagem de força bruta computacionalmente proibitiva.


4. Exemplo de Input Não-Trivial e Output
   
Este exemplo demonstra um sistema que é perfeitamente robusto a qualquer falha individual (K=1), mas que entra em colapso quando um par específico falha ($K=2$).

•	Cenário de Input:
  o	3 Submissões (S1, S2, S3), necessitando de 1 revisão cada. (Fluxo total necessário = 3).

  o	4 Revisores (R1, R2, R3, R4), cada um com uma capacidade máxima de 1 revisão.
  
  o	Ligações (Matches possíveis):
  
  	S1 está ligada a R1 e R2.
  
  	S2 está ligada a R3 e R4.
  
  	S3 está ligada a R1 e R4.
  
•	Execução (Verificação $K=1$): Se o R1 faltar sozinho, S1 usa R2, e S3 usa R4. (Fluxo = 3, Sucesso). Se o R4 faltar sozinho, S2 usa R3, e S3 usa R1. (Fluxo = 3, Sucesso). O sistema sobrevive a qualquer ausência isolada.

•	Execução (Verificação $K=2$): O algoritmo testa a combinação {R1, R4}. Se tanto o R1 como o R4 estiverem ausentes, os únicos revisores restantes são R2 e R3. A capacidade total da rede desce para 2, o que é insuficiente para cobrir as 3 submissões.

•	Output: O algoritmo identifica e devolve corretamente o par {R1, R4} como um grupo de risco crítico, evidenciando a correção de combinar a pesquisa combinatória com o fluxo em redes.
