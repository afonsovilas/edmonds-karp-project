#ifndef EDMONDSKARP_H
#define EDMONDSKARP_H

#include "data_structures/Graph.h"
#include <queue>
#include "models.h"
#include <vector>

template <class T>
void addFlowEdge(Graph<T> *g, const T &source, const T &dest, double capacity) {
    auto v1 = g->findVertex(source);
    auto v2 = g->findVertex(dest);

    if (v1 != nullptr && v2 != nullptr) {
        auto e1 = v1->addEdge(v2, capacity);
        auto e2 = v2->addEdge(v1, 0); // Aresta residual inicial a 0
        e1->setReverse(e2);
        e2->setReverse(e1);
    }
}

// O BFS para encontrar caminhos de aumento
template <class T>
bool findAugmentingPath(Graph<T> *g, Vertex<T> *s, Vertex<T> *t, Vertex<T>* ignoredNode = nullptr) {
    for (auto v : g->getVertexSet()) {
        v->setVisited(false);
    }

    s->setVisited(true);
    std::queue<Vertex<T>*> q;
    q.push(s);

    while (!q.empty() && !t->isVisited()) {
        auto v = q.front();
        q.pop();

        for (auto e : v->getAdj()) {
            Vertex<T> *w = e->getDest();
            double residual = e->getWeight() - e->getFlow();

            if (!w->isVisited() && residual > 0 && w != ignoredNode) {
                w->setVisited(true);
                w->setPath(e);
                q.push(w);
            }
        }
    }
    return t->isVisited();
}

template <class T>
void edmondsKarp(Graph<T> *g, const T &source, const T &target, Vertex<T>* ignoredNode = nullptr) {
    Vertex<T> *s = g->findVertex(source);
    Vertex<T> *t = g->findVertex(target);

    if (s == nullptr || t == nullptr) return;

    for (auto v : g->getVertexSet()) {
        for (auto e : v->getAdj()) {
            e->setFlow(0);
        }
    }

    while (findAugmentingPath(g, s, t, ignoredNode)) {
        double bottleneck = INF;
        Vertex<T> *v = t;
        while (v != s) {
            Edge<T> *e = v->getPath();
            double residual = e->getWeight() - e->getFlow();
            if (residual < bottleneck) bottleneck = residual;
            v = e->getOrig();
        }

        v = t;
        while (v != s) {
            Edge<T> *e = v->getPath();
            e->setFlow(e->getFlow() + bottleneck);
            if (e->getReverse() != nullptr) {
                e->getReverse()->setFlow(e->getReverse()->getFlow() - bottleneck);
            }
            v = e->getOrig();
        }
    }
}

template <class T>
std::vector<int> analiseDeRisco(Graph<T> *g, const T& sourceId, const T& sinkId, const std::vector<Reviewer>& revisores, int numSubmissoes, int minReviews) {
    std::vector<int> riskList;
    int revisoesNecessarias = numSubmissoes * minReviews;

    for (const auto& rev : revisores) {
        Vertex<T>* vRev = g->findVertex(rev.id);
        if (vRev == nullptr) continue;

        edmondsKarp(g, sourceId, sinkId, vRev);

        double fluxoTotal = 0;
        Vertex<T>* sink = g->findVertex(sinkId);
        if (sink != nullptr) {
            for (auto e : sink->getIncoming()) {
                fluxoTotal += e->getFlow();
            }
        }

        if (fluxoTotal < revisoesNecessarias) {
            riskList.push_back(rev.id);
        }
    }

    return riskList;
}

template <class T>
vector<Assignment> extrairAtribuicoes(Graph<T> *g, const vector<Reviewer>& revisores) {
    vector<Assignment> resultados;

    for (const auto& rev : revisores) {

        Vertex<T>* vRev = g->findVertex(rev.id);
        if (vRev == nullptr) continue;

        for (auto e : vRev->getAdj()) {

            if (e->getFlow() > 0) {
                Vertex<T>* vSub = e->getDest();

                Assignment a;
                a.reviewerId = rev.id;
                a.submissionId = vSub->getInfo();
                a.matchDomain = 0;

                resultados.push_back(a);
            }
        }
    }
    return resultados;
}

template <class T>
bool verificarSucesso(Graph<T> *g, const T& sinkId, int numSubmissoes, int minReviews) {
    double fluxoTotal = 0;

    Vertex<T>* sink = g->findVertex(sinkId);
    if (sink != nullptr) {
        for (auto e : sink->getIncoming()) {
            fluxoTotal += e->getFlow();
        }
    }

    int revisoesNecessarias = numSubmissoes * minReviews;

    cout << "-> Fluxo Máximo Alcançado: " << fluxoTotal << endl;
    cout << "-> Revisões Necessárias:   " << revisoesNecessarias << endl;

    if (fluxoTotal >= revisoesNecessarias) {
        cout << "SUCESSO: Todos os artigos têm os revisores mínimos alocados!" << endl;
        return true;
    } else {
        cout << "FALHA: Não foi possível satisfazer todas as revisões. Faltaram "
             << (revisoesNecessarias - fluxoTotal) << " revisões." << endl;
        return false;
    }
}

#endif //EDMONDSKARP_H