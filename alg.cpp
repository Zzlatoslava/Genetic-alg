#include <iostream>
#include <string>
#include <unordered_set>
#include <random>
// класс хромосомы Chromosome
#include <vector>
class Chromosome
{
private:
    std::vector<int> _workers; // вектор, хранящий назначенную рабочему задачу
    int _cost; // затраты
    int _size; // размер

public:
    Chromosome(){
    }

    Chromosome(std::mt19937 &rnd, int workers)
    {
        _size = workers;
        _workers = std::vector<int>(workers);
        Generate(rnd, workers);
    }

    Chromosome(int workers)
    {
        _size = workers;
        _workers = std::vector<int>(workers);
    }

    int Get_vector(std::vector<int> &vec)
    {
        vec.clear();
        vec =_workers;
        return vec[_size] ;

    }

    // скрещивание
    Chromosome Crossover(Chromosome &chr, std::mt19937 &rnd)
    {
        Chromosome child(_size);
        // генерируем index1 ,index2
        int index1 = rnd() % _size;  // остаток от деления сгенерированного числа на _size
        int index2 = rnd() % _size;
        int diff = std::abs(index1 - index2);

        while (index1 == index2 || diff + 1 >= _size)
        {
            index2 = rnd() % _size;
            diff = std::abs(index1 - index2);
        }

        if (index2 < index1)
        {
            int tmp = index1;
            index1 = index2;
            index2 = tmp;
        }

        for (int i = 0; i < index1; ++i)
        {
            int task = GetTask(i);
            child.Assign(i, task);
        }

        for (int i = index1; i <= index2; ++i)
        {
            int task = chr.GetTask(i);
            child.Assign(i, task);
        }

        for (int i = index2 + 1; i < _size; ++i)
        {
            int task = GetTask(i);
            child.Assign(i, task);
        }

        return child;
    }

    // функция мутации
    void Mutation(std::mt19937 &rnd,int ver)
    {
        for (int i = 0; i < _size; ++i)
        {
            // если остаток от деления этого случайного числа меньше 33,
            // то i-му рабочем назначается задача, полученная методом GetRandomTask(rnd, _size)
            if (rnd() % 100 < ver)
                Assign(i, GetRandomTask(rnd, _size));
        }
    }

    // осуществляет копирование обьекта
    void Copy(Chromosome &chr)
    {
        _cost = chr._cost;
        _size = chr._size;

        for (int i = 0; i < _size; ++i)
        {
            _workers[i] = chr._workers[i];
        }
    }

    int WorkerCost(int worker)
    {
        return _workers[worker];
    }

    int Cost()
    {
        return _cost;
    }

    int Size()
    {
        return _size;
    }

    void SetCost(int cost)
    {
        _cost = cost;
    }

    // функция назначает каждому рабочему задачу
    // (присваивает каждому эл-ту вектора  _workers по индексу worker значение task)
    void Assign(int worker, int task)
    {
        _workers[worker] = task;
    }

    int GetTask(int worker)
    {
        return _workers[worker];
    }

    int GetRandomTask(std::mt19937 &rnd, int taskRange)
    {
        return rnd() % taskRange;
    }

    void Generate(std::mt19937 &rnd, int taskRange)
    {
        // назначает каждому рабочему случайную задачу
        for (int count = 0; count < _size; ++count)
        {
            Assign(count, rnd() % taskRange);
        }
    }
};


// класс матрицы стоимостей
class CostMatrix
{
private:
    std::vector<std::vector<int>> _costArray; // матрица стоимостей
    int _n; // размер

public:
    CostMatrix(int agents)
    {
        _costArray = std::vector<std::vector<int>>(agents, std::vector<int>(agents));
        _n = agents;

        for (int i = 0; i < agents; ++i)
        {
            for (int j = 0; j < agents; ++j)
            {
                SetCost(i, j, 0);
            }
        }
    }

    CostMatrix(int agents, int cost)
    {
        _costArray = std::vector<std::vector<int>>(agents, std::vector<int>(agents));
        _n = agents;

        for (int i = 0; i < agents; ++i)
        {
            for (int j = 0; j < agents; ++j)
            {
                int value = cost * (j + 1);
                SetCost(i, j, value);
            }

            cost += 1;
        }
    }

    CostMatrix(int agents, int tasks, std::mt19937 &rnd)
    {
        _costArray = std::vector<std::vector<int>>(agents, std::vector<int>(tasks));
        _n = agents;

        for (int i = 0; i < agents; ++i)
        {
            for (int j = 0; j < tasks; ++j)
            {
                SetCost(i, j, rnd() % 91 + 10);
            }
        }
    }

    void SetCost(int agent, int task, int cost)
    {
        if (agent < _n && task < _n)
        {
            // присваиваем элементу с данным индексом данную стоимость
            _costArray[agent][task] = cost;
        }
    }

    int GetCost(int worker, int task)
    {
        // возвращает стоимость выполнения данного работника данной задачи
        return _costArray[worker][task];
    }

    // функция получения стоимости в хромосоме
    int GetChromosomeCost(Chromosome &chromosome, bool maximise)
    // возвращает суммарную стоимость всех элементов хромосомы
    {
        int totalCost = 0;
        std::unordered_set<int> assignments;

        for (int worker = 0; worker < _n; ++worker)
        {
            int task = chromosome.GetTask(worker);
            assignments.insert(task);
            totalCost += GetCost(worker, task);
        }

        int violations = _n - assignments.size();
        return maximise ? totalCost - violations * 100 : totalCost + violations * 100;
    }
};

// класс популяци
class Population
{
private:
    std::vector<Chromosome> _chromosomes; // вектор, хранящий объекты типа Chromosome
    long _bestChromosomeCost; // затраты на лучшую хромосому
    int _bestChromosomeIndex; // индекс лучшей хромосомы
    Chromosome _bestChromosome; // лучшая хромосома
    bool _maximise; // переменная для минимизации

public:
    Population(std::mt19937 &rnd, int populationSize, int taskSize, bool maximise = true)
    {
        _bestChromosomeCost = maximise ? -1 : 9999999999;
        _bestChromosomeIndex = -1;
        _chromosomes = std::vector<Chromosome>(populationSize);
        _maximise = maximise;
        CreateArbitraryPopulation(rnd, populationSize, taskSize);
    }

    // создаем произвольную популяцию
    void CreateArbitraryPopulation(std::mt19937 &rnd, int populationSize, int taskSize)
    // в качестве аргументов подаем генератор псевдослучайных чисел размер популяции и задачи
    {
        for (int i = 0; i < populationSize; ++i)
        {
            // обращаемся к каждому элементу вектора _chromosomes по индексу
            // и каждому рабочему присваеваем случайную задачу для данной популяции
            _chromosomes[i] = Chromosome(rnd, taskSize);
        }
    }

    // функция оценки
    void Evaluate(CostMatrix &costMatrix, int iteration, std::vector<int> &best,std::vector<int> &good1,
                  std::vector<int> &good2)
    {
        int bestCost = 0;
        int secondBestCost = 0;
        int thirdBestCost = 0;
        int bestIndex = -1;
        int secondBestIndex = -1;
        int thirdBestIndex = -1;
        int k ;
        //std::vector<std::vector<int>> &res;
        for (int i = 0; i < _chromosomes.size(); ++i)
        {
            // суммарная стоимость затрат хромосомы
            long cost = costMatrix.GetChromosomeCost(_chromosomes[i], _maximise);
            _chromosomes[i].SetCost(cost);

            if (IsBetter(cost, bestCost)) {
                // Сдвигаем второе и третье место вниз
                thirdBestCost = secondBestCost;
                thirdBestIndex = secondBestIndex;
                secondBestCost = bestCost;
                secondBestIndex = bestIndex;

                // Обновляем первое место
                bestCost = cost;
                bestIndex = i;
            } else if (IsBetter(cost, secondBestCost)) {
                // Сдвигаем третье место вниз
                thirdBestCost = secondBestCost;
                thirdBestIndex = secondBestIndex;

                // Обновляем второе место
                secondBestCost = cost;
                secondBestIndex = i;
            } else if (IsBetter(cost, thirdBestCost)) {
                // Обновляем третье место
                thirdBestCost = cost;
                thirdBestIndex = i;
            }
        }
        _chromosomes[bestIndex].Get_vector(best);
        _chromosomes[secondBestIndex].Get_vector(good1);
        _chromosomes[thirdBestIndex].Get_vector(good2);
    }

    // функция применения скрещивания
    void ApplyCrossover(std::mt19937 &rnd, int taskSize)
    {
        int size = _chromosomes.size();
        for (int i = 0; i < size; ++i)
        {
            int prob = rnd() % 100;
            if (prob < 50)
            {
                // выбираем два числа, как остаток от деления случайных чисел на размер хромосомы
                int index1 = rnd() % size;
                int index2 = rnd() % size;

                while (index1 == index2)
                {
                    index2 = rnd() % size;
                }
                // вызываем функцию, с помощью которой проводим одноточечное скрещивание
                Crossover(index1, index2, rnd, taskSize);
            }
        }
    }

    // ф-я скрещивания, в кач-ве аргументов принимает 2 родителей, случайное число и размер задачи
    void Crossover(int parentIndex1, int parentIndex2, std::mt19937 &rnd, int taskSize)
    {
        // выполняем одтоточесное скрещивание
        Chromosome chr1 = _chromosomes[parentIndex1];
        Chromosome chr2 = _chromosomes[parentIndex2];

        Chromosome child1 = chr1.Crossover(chr2, rnd);
        Chromosome child2 = chr2.Crossover(chr1, rnd);
        // обращаемся к элементу вектора _chromosomes по индексу родителя parentIndex1
        // и записываем в него скопированный child1
        _chromosomes[parentIndex1].Copy(child1);
        _chromosomes[parentIndex2].Copy(child2);
    }

    // функция мутации, в кач-ве аргумента принимает случайное число
    void Mutate(std::mt19937 &rnd,int ver)
    {
        for (int i = 0; i < _chromosomes.size(); ++i)
        {
            // для каждой хромосомы считаем число prob,
            // определенное как остаток от деления сгенерированного числа на 100
            int prob = rnd() % 100;
            if (prob < 5)
            {
                // для каждого элемента вектора _chromosomes вызываем функцию Mutation
                _chromosomes[i].Mutation(rnd,ver);
            }
        }
    }

    void StoreBestSolution(int taskSize)
    {
        _bestChromosome = Chromosome(taskSize);
        _bestChromosome.Copy(_chromosomes[_bestChromosomeIndex]);
    }


    void SeedBestSolution(std::mt19937 &rnd)
    {
        // определяется остатком от деления случайного числа на размер вектора  _chromosomes
        int index = rnd() % _chromosomes.size();
        // пока значения индекса совпадает со значение индекса наилучшей хромосомы
        while (index == _bestChromosomeIndex)
        {
            index = rnd() % _chromosomes.size();
        }
        _chromosomes[index].Copy(_bestChromosome);
    }

    // ф-я для определения лучшей стоимости
    bool IsBetter(long cost1, long cost2)
    {
        return _maximise ? cost1 > cost2 : cost1 < cost2;
    }

    // функция отбора
    void Selection(std::mt19937 &rnd)
    {
        // размер вектора _chromosomes
        int size = _chromosomes.size();

        for (int i = 0; i < size; ++i)
        {
            int prob = rnd() % 100;
            if (prob < 20)
            {
                int index1 = rnd() % size;
                int index2 = rnd() % size;

                while (index1 == index2)
                {
                    index2 = rnd() % size;
                }

                // переменная cost1 определяется как стоимость хромосомы
                long cost1 = _chromosomes[index1].Cost();
                long cost2 = _chromosomes[index2].Cost();

                if (IsBetter(cost1, cost2))
                {
                    // в элемент вектора _chromosomes по индексу index2 копируем элемент,
                    // хранящийся по индексу index1
                    _chromosomes[index2].Copy(_chromosomes[index1]);
                }

                else
                {
                    _chromosomes[index1].Copy(_chromosomes[index2]);
                }
            }
        }
    }
};

/*
int main(){
    int tasks = 5;
    int popSize = 100;
    std::mt19937 rnd(std::random_device{}());
    bool maximise = false;
    CostMatrix matrix(tasks);

    matrix.SetCost(0, 0, 11);
    matrix.SetCost(0, 1, 7);
    matrix.SetCost(0, 2, 10);
    matrix.SetCost(0, 3, 17);
    matrix.SetCost(0, 4, 10);

    matrix.SetCost(1, 0, 13);
    matrix.SetCost(1, 1, 21);
    matrix.SetCost(1, 2, 7);
    matrix.SetCost(1, 3, 11);
    matrix.SetCost(1, 4, 13);

    matrix.SetCost(2, 0, 13);
    matrix.SetCost(2, 1, 13);
    matrix.SetCost(2, 2, 15);
    matrix.SetCost(2, 3, 13);
    matrix.SetCost(2, 4, 14);

    matrix.SetCost(3, 0, 18);
    matrix.SetCost(3, 1, 10);
    matrix.SetCost(3, 2, 13);
    matrix.SetCost(3, 3, 16);
    matrix.SetCost(3, 4, 14);

    matrix.SetCost(4, 0, 12);
    matrix.SetCost(4, 1, 8);
    matrix.SetCost(4, 2, 16);
    matrix.SetCost(4, 3, 19);
    matrix.SetCost(4, 4, 10);

    Population _population(rnd, popSize, tasks, maximise);
    int iteration = 1;
    _population.Evaluate(matrix, iteration);

    while (iteration < 100)
    {
        _population.StoreBestSolution(tasks);
        _population.Mutate(rnd);
        _population.ApplyCrossover(rnd, tasks);
        _population.SeedBestSolution(rnd);
        _population.Evaluate(matrix, iteration);
        _population.Selection(rnd);
        iteration++;
    }
    return 0;
}
*/

