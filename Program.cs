using System;

public class TreeNode<T>
{
    public T Value { get; set; }
    public TreeNode<T> Left { get; set; }
    public TreeNode<T> Right { get; set; }

    public TreeNode(T value)
    {
        Value = value;
        Left = null;
        Right = null;
    }
}

public class BinarySearchTree<T> where T : IComparable<T>
{
    public TreeNode<T> Root { get; private set; }
    private int count = 0;
    private const int MaxElements = 20;
    private readonly bool allowDuplicates;

    public BinarySearchTree(bool allowDuplicates)
    {
        this.allowDuplicates = allowDuplicates;
    }

    public bool Insert(T value)
    {
        if (count >= MaxElements)
        {
            Console.WriteLine("Превышено максимальное количество элементов (20).");
            return false;
        }

        bool inserted = false;
        Root = InsertRec(Root, value, ref inserted);

        if (inserted) count++;
        else if (!allowDuplicates)
            Console.WriteLine("Значение уже существует в дереве и не будет добавлено.");

        return inserted;
    }

    private TreeNode<T> InsertRec(TreeNode<T> node, T value, ref bool inserted)
    {
        if (node == null)
        {
            inserted = true;
            return new TreeNode<T>(value);
        }

        int comparison = value.CompareTo(node.Value);
        if (comparison < 0)
        {
            node.Left = InsertRec(node.Left, value, ref inserted);
        }
        else if (comparison > 0)
        {
            node.Right = InsertRec(node.Right, value, ref inserted);
        }
        else if (allowDuplicates)
        {
            node.Right = InsertRec(node.Right, value, ref inserted);
        }

        return node;
    }

    public bool Contains(T value)
    {
        return ContainsRec(Root, value);
    }

    private bool ContainsRec(TreeNode<T> node, T value)
    {
        if (node == null) return false;

        int comparison = value.CompareTo(node.Value);
        if (comparison == 0) return true;

        return comparison < 0 ? ContainsRec(node.Left, value) : ContainsRec(node.Right, value);
    }

    public int CountOccurrences(T value)
    {
        return CountOccurrencesRec(Root, value);
    }

    private int CountOccurrencesRec(TreeNode<T> node, T value)
    {
        if (node == null) return 0;

        int comparison = value.CompareTo(node.Value);
        if (comparison == 0)
        {
            return 1 + CountOccurrencesRec(node.Right, value);
        }
        else if (comparison < 0)
        {
            return CountOccurrencesRec(node.Left, value);
        }
        else
        {
            return CountOccurrencesRec(node.Right, value);
        }
    }

    public void PrintTree()
    {
        if (Root == null)
        {
            Console.WriteLine("[пусто]");
        }
        else
        {
            PrintTreeRec(Root, "", false, true);
        }
    }



    private void PrintTreeRec(TreeNode<T> node, string indent, bool isRight, bool isRoot)
    {
        if (node == null)
            return;

        string newIndent = indent + "    ";

        if (node.Right != null)
            PrintTreeRec(node.Right, newIndent, true, false);

        Console.Write(indent);
        if (!isRoot)
            Console.Write(isRight ? "┌── " : "└── ");
        Console.WriteLine(node.Value);

        if (node.Left != null)
            PrintTreeRec(node.Left, newIndent, false, false);
    }
}

class Program
{
    static void Main()
    {
        Console.Write("Разрешить ввод повторяющихся значений? (да/нет): ");
        string allowDuplicatesInput = Console.ReadLine().Trim().ToLower();
        bool allowDuplicates = allowDuplicatesInput == "да";

        BinarySearchTree<int> tree = new BinarySearchTree<int>(allowDuplicates);
        Console.WriteLine("\nВведите до 20 чисел для вставки в дерево. Для завершения введите пустую строку.");

        while (true)
        {
            Console.Write("Введите число: ");
            string input = Console.ReadLine();

            if (string.IsNullOrWhiteSpace(input)) break;

            if (int.TryParse(input, out int value))
            {
                tree.Insert(value);
            }
            else
            {
                Console.WriteLine("Некорректный ввод.");
            }
        }

        Console.WriteLine("\nДерево (повернутое на 90 градусов, корень слева):");
        tree.PrintTree();

        Console.Write("\nВведите значение для поиска: ");
        if (int.TryParse(Console.ReadLine(), out int searchVal))
        {
            bool found = tree.Contains(searchVal);
            Console.WriteLine(found ? "Значение найдено." : "Значение не найдено.");
        }

        if (allowDuplicates)
        {
            Console.Write("\nВведите значение для подсчета вхождений: ");
            if (int.TryParse(Console.ReadLine(), out int countVal))
            {
                int occurrences = tree.CountOccurrences(countVal);
                Console.WriteLine($"Количество вхождений: {occurrences}");
            }
        }
        else
        {
            Console.WriteLine("\nПодсчёт количества вхождений отключен (дубликаты не разрешены).");
        }
    }
}
