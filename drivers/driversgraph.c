#include <stdio.h>
#include "GraphPage.h"

static int total = 0;
static int gagal = 0;
static Graph g;
static Graph masuk;

static void cek(bool kondisi, const char *nama){
    total++;
    if (!kondisi){
        gagal++;
        printf("Test %s gagal\n", nama);
    }
}

static void kosongkan(Graph *x){
    int i;
    for (i = 0; i < x->length_vertex; i++){
        deleteAllPage(&x->node[i].Edge);
    }
    x->length_vertex = 0;
}

static bool tetangga(const Graph *x, int dari, const int *harapan, int n){
    int idx = SearchVertexIdx(*x, dari);
    int i = 0;
    AddresslistPage p;
    if (idx == INVALID_IDX) return false;
    p = x->node[idx].Edge;
    while (p != NULL && i < n){
        if (p->info != harapan[i]) return false;
        p = p->next;
        i++;
    }
    return p == NULL && i == n;
}

int main(void){
    Vertex v;
    int e1[] = {2, 3};
    int e2[] = {3};
    int e3[] = {1};
    int i;

    createGraph(&g);
    cek(g.length_vertex == 0, "create kosong");
    cek(SearchVertexIdx(g, 1) == INVALID_IDX, "cari di graph kosong");

    v = newVertex(5);
    cek(v.info == 5 && v.Edge == NULL, "newVertex");

    insertVertex(&g, 1);
    insertVertex(&g, 2);
    insertVertex(&g, 3);
    cek(g.length_vertex == 3, "tiga vertex");
    cek(SearchVertexIdx(g, 1) == 0, "indeks vertex 1");
    cek(SearchVertexIdx(g, 2) == 1, "indeks vertex 2");
    cek(SearchVertexIdx(g, 3) == 2, "indeks vertex 3");
    cek(SearchVertexIdx(g, 9) == INVALID_IDX, "vertex tidak ada");
    cek(g.node[0].Edge == NULL, "edge awal kosong");

    addEdge(&g, 1, 3);
    addEdge(&g, 1, 2);
    cek(tetangga(&g, 1, e1, 2), "edge 1->2 dan 1->3 terurut");
    cek(g.node[1].Edge == NULL && g.node[2].Edge == NULL, "graf berarah: tujuan tidak punya edge balik");

    addEdge(&g, 2, 3);
    cek(tetangga(&g, 2, e2, 1), "edge 2->3");
    addEdge(&g, 3, 1);
    cek(tetangga(&g, 3, e3, 1), "edge 3->1 (siklus)");

    addEdge(&g, 1, 9);
    cek(tetangga(&g, 1, e1, 2), "edge ke vertex tidak ada diabaikan");
    addEdge(&g, 9, 1);
    cek(g.length_vertex == 3, "edge dari vertex tidak ada diabaikan");

    deleteEdge(&g, 1, 2);
    cek(tetangga(&g, 1, e2, 1), "hapus edge 1->2");
    cek(tetangga(&g, 2, e2, 1), "edge lain tidak berubah");
    deleteEdge(&g, 1, 2);
    cek(tetangga(&g, 1, e2, 1), "hapus edge yang tidak ada");
    deleteEdge(&g, 1, 9);
    deleteEdge(&g, 9, 1);
    cek(tetangga(&g, 1, e2, 1), "hapus edge vertex tidak ada");
    deleteEdge(&g, 1, 3);
    cek(g.node[0].Edge == NULL, "semua edge 1 terhapus");

    kosongkan(&g);

    createGraph(&g);
    createGraph(&masuk);
    for (i = 1; i <= 3; i++){
        insertVertex(&g, i);
        insertVertex(&masuk, i);
    }
    addEdge(&g, 2, 3);
    addEdge(&masuk, 3, 2);
    addEdge(&g, 1, 3);
    addEdge(&masuk, 3, 1);
    cek(lengthListPage(g.node[SearchVertexIdx(g, 2)].Edge) == 1, "link keluar P2");
    cek(lengthListPage(masuk.node[SearchVertexIdx(masuk, 3)].Edge) == 2, "backlink P3 dari dua halaman");
    deleteEdge(&g, 2, 3);
    deleteEdge(&masuk, 3, 2);
    cek(g.node[SearchVertexIdx(g, 2)].Edge == NULL, "link keluar P2 dihapus");
    cek(lengthListPage(masuk.node[SearchVertexIdx(masuk, 3)].Edge) == 1, "backlink P3 tinggal satu");
    kosongkan(&g);
    kosongkan(&masuk);

    createGraph(&g);
    for (i = 1; i <= 1001; i++){
        insertVertex(&g, i);
    }
    cek(g.length_vertex == 1000, "kapasitas 1000 vertex");
    cek(SearchVertexIdx(g, 1) == INVALID_IDX, "vertex paling lama terbuang saat penuh");
    cek(SearchVertexIdx(g, 2) == 0, "vertex 2 bergeser ke indeks 0");
    cek(SearchVertexIdx(g, 1001) == 999, "vertex baru di indeks terakhir");
    kosongkan(&g);

    printf("Graph driver: %d/%d lulus\n", total - gagal, total);
    if (gagal == 0) return 0;
    return 1;
}