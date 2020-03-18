#include <iostream>
#include<vector>
#include<list>
using namespace std;
class graf_neorientat {
	int** matrice, nr_noduri;
public:
	graf_neorientat();
	graf_neorientat(int** matrice, int nr_noduri);
	graf_neorientat(const graf_neorientat& G);
	~graf_neorientat();
	friend istream& operator>>(istream& os, graf_neorientat& G);
	friend ostream& operator<<(ostream& os, const graf_neorientat& G);
	void citire_n_elemente(int n);
	void DFSUtil(int curent, bool visited[]);
	void DFS(int start, bool visited[]);
	void BFS(int start);
	void matricea_extinsa();
	void is_conex();
	void componente_conexe();
	graf_neorientat operator +(const graf_neorientat& G);
};
struct muchie {
	int nod1, nod2;
};

// constructor initializare fara parametrii
graf_neorientat::graf_neorientat() {
	int nr_noduri = NULL;
}

// constructor initializare cu parametrii
graf_neorientat::graf_neorientat(int** matrice, int nr_noduri) {
	this->nr_noduri = nr_noduri;
	this->matrice = new int* [nr_noduri];
	for (int i = 0; i < nr_noduri; i++)
		this->matrice[i] = new int[nr_noduri];
	for (int i = 0; i < nr_noduri; i++)
		for (int j = 0; j < nr_noduri; j++)
			this->matrice[i][j] = matrice[i][j];
}

// constructor copiere
graf_neorientat::graf_neorientat(const graf_neorientat& G) {
	nr_noduri = G.nr_noduri;
	matrice = new int* [nr_noduri];
	for (int i = 0; i < nr_noduri; i++) {
		matrice[i] = new int[nr_noduri];
		for (int j = 0; j < nr_noduri; j++)
			matrice[i][j] = G.matrice[i][j];
	}
}

// citire
istream& operator >>(istream& os, graf_neorientat& G) {
	os >> G.nr_noduri;
	for (int i = 0; i < G.nr_noduri; i++)
		for (int j = 0; j < G.nr_noduri; j++)
			os >> G.matrice[i][j];
	return os;
}
// afisare

ostream& operator << (ostream& os, const graf_neorientat& G) {
	//Matricea de adiacenta
	os << "Matrice de adiacenta:\n";
		for (int i = 0; i < G.nr_noduri; i++) {
			for (int j = 0; j < G.nr_noduri; j++)
				os << G.matrice[i][j] << " ";
			os << "\n";
		}
	os << "\n";

	//Lista de muchii
	os << "\nLista de muchii:\n";
	vector<muchie> v;
	for (int i = 0; i <= G.nr_noduri - 2; i++) {
		for (int j = i + 1; j <= G.nr_noduri - 1; j++)
			if (G.matrice[i][j] == 1) {
				muchie m;
				m.nod1 = i;
				m.nod2 = j;
				v.push_back(m);
			}
	}
	os << '(';
	for (muchie& e : v) os << '[' << e.nod1 << ',' << e.nod2 << ']' << ' ';
	os << ')' << "\n";

	//Liste de adiacente (de vecini)
	os << "\nListe de adiacenta:\n";
	vector<list<int>> mips(G.nr_noduri);
	for (int i = 0; i < G.nr_noduri; i++)
		for (int j = 0; j < G.nr_noduri; j++)
			if (G.matrice[i][j] == 1)
				mips[i].push_back(j);
	for (int i = 0; i < G.nr_noduri; i++) {
		os << i << ": ";
		for (auto p = mips[i].begin(); p != mips[i].end(); ++p)
			os << ' ' << *p;
		os << "\n";
	}
	return os;
}

// citire si afisare de n grafuri neorientate
void graf_neorientat::citire_n_elemente(int n) {
	graf_neorientat* G;
	G = new graf_neorientat[n];
	for (int i = 0; i < n; i++)
		cin >> G[i];
	for (int j = 0; j < n; j++)
		cout << G[j];
}

graf_neorientat::~graf_neorientat() {
	for (int i = 0; i < nr_noduri; i++)
		delete[] matrice[i];
	delete[] matrice;
	nr_noduri = NULL;
}

// parcurgerea in adancime 
void graf_neorientat::DFSUtil(int curent, bool visited[]) {
	visited[curent] = true;
	cout << curent << " ";
	for (int i = 0; i < nr_noduri; i++)
		if (matrice[curent][i] and visited[i] != true)
			DFSUtil(i, visited);
}

void graf_neorientat::DFS(int start, bool visited[]) {
	for (int i = 0; i < nr_noduri; i++)
		visited[i] = false;
	cout << "DFS: ";
		DFSUtil(start, visited);
	cout << "\n";
}

// parcurgerea in latime
void graf_neorientat::BFS(int start) {
	int* queue, ic = 0, fc = 0, curent;
	bool* visited = new bool[nr_noduri];
	for (int i = 0; i < nr_noduri; i++)
		visited[i] = false;

	queue = new int[nr_noduri];
	visited[start] = true;
	queue[ic] = start;
	cout << "BFS= ";
	while (ic <= fc) {
		curent = queue[ic];
		cout << curent << " ";
			for (int i = 0; i < nr_noduri; i++)
				if (matrice[curent][i] and visited[i] == false) {
					fc++;
					queue[fc] = i;
					visited[i] = true;
				}
		ic++;
	}
}


// determinarea matricii (existentei) drumurilor
void graf_neorientat::matricea_extinsa() {
	int** a;
	a = new int* [nr_noduri];
	for (int i = 0; i < nr_noduri; i++) {
		a[i] = new int[nr_noduri];
		for (int j = 0; j < nr_noduri; j++)
			a[i][j] = matrice[i][j];
	}
	for (int k = 0; k < nr_noduri; ++k)
		for (int i = 0; i < nr_noduri; ++i)
			for (int j = 0; j < nr_noduri; ++j)
				if (i != j and a[i][j] == 0 and a[i][k] == 1 and a[k][j] == 1)
					a[i][j] = 1;
}

// determinarea componentelor conexe nu ca grafuri, ci ca liste de noduri

void  graf_neorientat::componente_conexe() {
	bool* visited;
	visited = new bool[nr_noduri];
	for (int i = 0; i < nr_noduri; i++)
		if (visited[i] == 0)
			DFS(i, visited);
}


void graf_neorientat::is_conex() {
	bool* visited;
	visited = new bool[nr_noduri];
	DFS(0, visited);
	bool sem = true;
	for (int i = 1; i < nr_noduri; i++)
		if (visited[i] == false) {
			sem = false;
			break;
		}
	if (sem)
		cout << "\nGraful este conex\n";
	else
		cout << "\nGraful nu este conex\n";
}

graf_neorientat graf_neorientat::operator +(const graf_neorientat& G1) {
		int** a = new int* [nr_noduri];
		for (int i = 0; i < nr_noduri; i++) {
			a[i] = new int[nr_noduri];
			for (int j = 0; j < nr_noduri; j++)
				if (matrice[i][j] == 1 || G1.matrice[i][j] == 1)
					a[i][j] = 1;
				else
					a[i][j] = 0;
		}
		graf_neorientat G2(a, nr_noduri);
		return G2;
}
int main() {
	int n;
	cin >> n;
	int** a = new int* [n];
	for (int i = 0; i < n; i++)
		a[i] = new int[n];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> a[i][j];
	graf_neorientat G(a, n);
	cout << G;

	G.matricea_extinsa();
	G.is_conex();
	G.componente_conexe();
	int** b = new int* [n];
	for (int i = 0; i < n; i++)
		b[i] = new int[n];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> b[i][j];
	graf_neorientat G1(b, n);
	graf_neorientat G2(G+G1);
	cout << G2;
}
