#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
//------------classe Produto--------------------------------------------

class Produto {
public:
  Produto(int codigo, const std::string &nome, int qtde, double valorUnitario)
      : codigo(codigo), nome(nome), qtde(qtde), valorUnitario(valorUnitario) {}

  int codigo;
  string nome;
  float valorUnitario;
  int qtde;
  Produto *esq;
  Produto *dir;

  int getCodigo() const { return codigo; }

  const std::string getNome() const { return nome; }

  int getQuantidade() const { return qtde; }

  double getValorUnitario() const { return valorUnitario; }

private:
  // int codigo;
  // std::string nome;
  // int qtde;
  // double valorUnitario;
};

//----------------------------Classe
//NodeAVL--------------------------------------

class Node {
public:
  Produto produto;
  int height;
  Node *left;
  Node *right;
  Node *parent; // novo membro para rastrear o pai
  Node(Produto p)
      : produto(p), left(nullptr), right(nullptr), parent(nullptr), height(1) {}
};

void clearNodes(Node *node) {
  if (node == nullptr) {
    return;
  }
  clearNodes(node->left);
  clearNodes(node->right);
  delete node;
}

//----------------------------------Classe
//AVL----------------------------------------

class AVLTree {
public:
  Node *getRoot() { return root; }

  double calculateTotalValue() { return calculateTotalValue(root); }

  void displayLessThanQuantidade(int quantidade, Node *node) {
    if (node != nullptr) {
      displayLessThanQuantidade(quantidade, node->left);
      if (node->produto.getQuantidade() < quantidade) {
        cout << "Codigo: " << node->produto.getCodigo() << endl;
        cout << "Nome: " << node->produto.getNome() << endl;
        cout << "Quantidade: " << node->produto.getQuantidade() << endl;
        cout << "Valor unitario: " << node->produto.getValorUnitario() << endl;
      }
      displayLessThanQuantidade(quantidade, node->right);
    }
  }

  AVLTree() : root(nullptr) {}

  void insert(Produto p) { root = insertNode(root, p); }

  Node *search(int codigo) { return searchNode(root, codigo); }

  void display() { displayTree(root, 0); }

  void clear() {
    clearNodes(root);
    root = nullptr;
  }

  void remove(int codigo) { root = removeNode(root, codigo); }

  Node *removeNode(Node *node, int codigo) {
    if (node == nullptr) {
      return nullptr;
    }

    if (codigo < node->produto.getCodigo()) {
      node->left = removeNode(node->left, codigo);
    } else if (codigo > node->produto.getCodigo()) {
      node->right = removeNode(node->right, codigo);
    } else {
      if (node->left == nullptr && node->right == nullptr) {
        delete node;
        return nullptr;
      } else if (node->left == nullptr) {
        Node *temp = node->right;
        delete node;
        return temp;
      } else if (node->right == nullptr) {
        Node *temp = node->left;
        delete node;
        return temp;
      } else {
        Node *temp = findMinNode(node->right);
        node->produto = temp->produto;
        node->right = removeNode(node->right, temp->produto.getCodigo());
      }
    }

    return rebalance(node);
  }

  Node *findMinNode(Node *node) {
    while (node->left != nullptr) {
      node = node->left;
    }
    return node;
  }

  void printAllProducts() { printAllProducts(root); }

  int countNodes(Node *root) {
    if (root == nullptr) {
      return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
  }

private:
  Node *root;

  int height(Node *node) {
    if (node == nullptr) {
      return 0;
    }
    return node->height;
  }

  int balanceFactor(Node *node) {
    if (node == nullptr) {
      return 0;
    }
    return height(node->left) - height(node->right);
  }

  void updateHeight(Node *node) {
    node->height = 1 + max(height(node->left), height(node->right));
  }

  Node *rotateRight(Node *node) {
    Node *newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
  }

  Node *rotateLeft(Node *node) {
    Node *newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
  }

  Node *rebalance(Node *node) {
    updateHeight(node);

    int bf = balanceFactor(node);

    if (bf > 1) {
      if (balanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
      }
      return rotateRight(node);
    } else if (bf < -1) {
      if (balanceFactor(node->right) > 0) {
        node->right = rotateRight(node->right);
      }
      return rotateLeft(node);
    }

    return node;
  }

  Node *insertNode(Node *node, Produto p) {
    if (node == nullptr) {
      return new Node(p);
    }

    if (p.getCodigo() < node->produto.getCodigo()) {
      node->left = insertNode(node->left, p);
    } else if (p.getCodigo() > node->produto.getCodigo()) {
      node->right = insertNode(node->right, p);
    } else {
      return node;
    }

    return rebalance(node);
  }

  Node *searchNode(Node *node, int codigo) {
    if (node == nullptr) {
      return nullptr;
    }

    if (codigo == node->produto.getCodigo()) {
      return node;
    } else if (codigo < node->produto.getCodigo()) {
      return searchNode(node->left, codigo);
    } else {
      return searchNode(node->right, codigo);
    }
  }

  void printAllProducts(Node *node) {
    if (node == nullptr) {
      return;
    }

    printAllProducts(node->left);

    cout << "codigo(" << node->produto.getCodigo()
         << ") "
         //<< "antecessor:" << (node->parent != nullptr ?
         //node->parent->produto.getCodigo() : -1) << " "
         << "filho_esquerda:"
         << (node->left != nullptr ? node->left->produto.getCodigo() : -1)
         << " "
         << "filho_direita:"
         << (node->right != nullptr ? node->right->produto.getCodigo() : -1)
         << " "
         << "Degree: "
         << (node->left != nullptr ? 1 : 0) + (node->right != nullptr ? 1 : 0)
         << " "
         << "nivel: " << getLevel(node) << " "
         << "Height: " << node->height << " "
         << "balanceFactor: " << balanceFactor(node) << endl;

    printAllProducts(node->right);
  }

  int getLevel(Node *node) {
    if (node == nullptr) {
      return -1;
    }
    return 1 + max(getLevel(node->left), getLevel(node->right));
  }

  void displayTree(Node *node, int indent) {
    if (node == nullptr) {
      return;
    }

    displayTree(node->right, indent + 4);

    cout << string(indent, ' ') << node->produto.getCodigo() << endl;

    if (node->right != nullptr) {
      cout << string(indent, ' ') << "R: " << node->right->produto.getCodigo()
           << endl;
    }

    if (node->left != nullptr) {
      cout << string(indent, ' ') << "L: " << node->left->produto.getCodigo()
           << endl;
    }

    displayTree(node->left, indent + 4);
  }

  double calculateTotalValue(Node *node) {
    if (node == nullptr) {
      return 0.0;
    }
    double total =
        node->produto.getQuantidade() * node->produto.getValorUnitario();
    total += calculateTotalValue(node->left);
    total += calculateTotalValue(node->right);
    return total;
  }
};

//-------------------------------programa
//principal------------------------------------------

int main() {
  AVLTree tree;
  Produto *root = nullptr;

  tree.insert(Produto(5, "jhgsdg", 10, 10));
  tree.insert(Produto(3, "sdfsdfgsdf", 12, 9));
  tree.insert(Produto(10, "xccvcvcv", 23, 12));
  tree.insert(Produto(6, "jhghjg", 4, 12));
  tree.insert(Produto(1, "rffvvev", 23, 20));
  tree.insert(Produto(2, "ddsdc", 11, 5));
  tree.insert(Produto(12, "wecssxc", 10, 13));
  tree.insert(Produto(8, "jyujyuj", 6, 10));
  tree.insert(Produto(9, "fgbbdddd", 7, 10));
  tree.insert(Produto(4, "jhccbvvbvgsdg", 48, 9));
  tree.insert(Produto(13, "iuouy", 14, 17));
  tree.insert(Produto(11, "ppppyy", 10, 14));

  while (true) {
    int choice;
    cout << "Selecione uma opcao:" << endl;
    cout << "1 - Inserir produto" << endl;
    cout << "2 - Buscar produto por codigo" << endl;
    cout << "3 - Exibir arvore AVL" << endl;
    cout << "4 - Limpar arvore" << endl;
    cout << "5 - Remover um produto" << endl;
    cout << "6 - Valor de um item do estoque" << endl;
    cout << "7 - Valor total do estoque" << endl;
    cout << "8 - Produtos com estoque baixo" << endl;
    cout << "0 - Sair" << endl;
    cin >> choice;

    switch (choice) {
    case 1: {
      int codigo, qtde;
      float valorUnitario;
      string nome;
      cout << "Informe o codigo do produto: ";
      cin >> codigo;
      cout << "Informe o nome do produto: ";
      cin >> nome;
      cout << "Informe a quantidade em estoque: ";
      cin >> qtde;
      cout << "Informe o valor unitario do produto: ";
      cin >> valorUnitario;
      tree.insert(Produto(codigo, nome, qtde, valorUnitario));
      cout << "Produto inserido com sucesso!" << endl;
      break;
    }
    case 2: {
      int codigo;
      cout << "Informe o codigo do produto a ser buscado: ";
      cin >> codigo;
      Node *node = tree.search(codigo);
      cout << "Produto encontrado na posicao " << node->produto.getCodigo()
           << endl;
      cout << "Dados do produto: " << endl;
      cout << "Codigo: " << node->produto.getCodigo() << endl;
      cout << "Nome: " << node->produto.getNome() << endl;
      cout << "Valor unitario: R$" << node->produto.getValorUnitario() << endl;
      cout << "Quantidade em estoque: " << node->produto.getQuantidade()
           << endl;
      cout << "Valor total do item: R$"
           << node->produto.getQuantidade() * node->produto.getValorUnitario()
           << endl;
      cout << "Filho esquerda: "
           << (node->left ? to_string(node->left->produto.getCodigo()) : "null")
           << endl;
      cout << "Filho direita: "
           << (node->right ? to_string(node->right->produto.getCodigo())
                           : "null")
           << endl;
      break;
    }
    case 3: {
      int qtdNos = tree.countNodes(tree.getRoot());
      cout << "A arvore tem " << qtdNos << " nos." << endl;
      tree.display();

      cout << "Lista de produtos armazenados:" << endl;
      tree.printAllProducts();
      break;
    }
    case 4: {
      // Limpa a �rvore
      tree.clear();
      cout << "Arvore limpa.... " << endl;
      break;
    }
    case 5: {
      int codigo;
      cout << "Informe o codigo do produto a ser removido: ";
      cin >> codigo;
      tree.remove(codigo);
      cout << "Produto removido com sucesso....." << endl;
      break;
    }
    case 6: {
      int codigo;
      cout << "Informe o codigo do produto a ser buscado: ";
      cin >> codigo;
      Node *node = tree.search(codigo);
      cout << "Codigo: " << node->produto.getCodigo() << endl;
      cout << "Nome: " << node->produto.getNome() << endl;
      cout << "Valor total do item: R$"
           << node->produto.getQuantidade() * node->produto.getValorUnitario()
           << endl;
      double totalValue = tree.calculateTotalValue();
      cout << "Percentagem em relacao ao total do estoque:  "
           << node->produto.getQuantidade() * node->produto.getValorUnitario() /
                  totalValue * 100
           << "%" << endl;

      break;
    }
    case 7: {
      double totalValue = tree.calculateTotalValue();
      cout << "Valor total do estoque: R$ " << totalValue << endl;
      break;
    }
    case 8: {
      int quanto;
      cout << "Informe o valor de referencia: ";
      cin >> quanto;
      tree.displayLessThanQuantidade(quanto, tree.getRoot());
      break;
    }
    case 0: {
      return 0;
    }
    default: {
      cout << "Opcao invalida." << endl;
      break;
    }
    }
  }
}
