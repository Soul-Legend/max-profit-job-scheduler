#include <algorithm>
#include <vector>

using namespace std;

// O(n log n).
int jobScheduling(vector<int> &startTime, vector<int> &endTime,
                  vector<int> &profit) {
  // Cada evento representa a abertura ou o fechamento de um job e contém o
  // tempo de ocorrência do evento e o id do job.
  struct Event {
    int time;
    int job_id;
  };

  // A ordenação será em ordem decrescente para usar o pop_back() do std::vector
  // que tem complexidade O(1).
  constexpr auto cmp_event = [](const Event &a, const Event &b) {
    if (a.time != b.time) {
      return a.time > b.time;
    }
    return a.job_id > b.job_id;
  };

  int num_jobs = startTime.size();

  // Openings guarda as aberturas e closings guarda os encerramentos dos jobs.
  vector<Event> openings(num_jobs), closings(num_jobs);
  for (int job_id = 0; job_id < num_jobs; ++job_id) {
    openings[job_id] = {startTime[job_id], job_id};
    closings[job_id] = {endTime[job_id], job_id};
  }
  sort(openings.begin(), openings.end(), cmp_event);
  sort(closings.begin(), closings.end(), cmp_event);

  // Dp (programação dinâmica) de i representa o lucro máximo possível caso se
  // escolha realizar o job i, considerando somente os jobs não-conflitantes que
  // precedem i. Iniciamente, o lucro máximo de um job i é somente o lucro do
  // job i.
  vector<int> dp = profit;

  // Global_max_profit é o lucro máximo em relação a todos os jobs.
  // Cur_max_profit é o lucro máximo em relação aos jobs que precedem o job
  // atual (não inclui o lucro do job atual).
  int global_max_profit = 0, cur_max_profit = 0;
  while (!openings.empty()) {
    // Para cada abertura do job atual j que ocorra depois do fechamento de um
    // job i (ou seja, é possível realizar os dois jobs).
    while (openings.back().time >= closings.back().time) {
      // Verifique se vale a pena trocar o lucro gerado pela combinação atual de
      // jobs (cur_max_profit) pelo lucro máximo ao se escolher o job i (dp[job
      // i]).
      cur_max_profit = max(cur_max_profit, dp[closings.back().job_id]);

      closings.pop_back();
    }

    // Registre o lucro máximo obtido para o job atual (dp[job atual] já está
    // acrescido do valor de profit[job atual]).
    dp[openings.back().job_id] += cur_max_profit;

    global_max_profit = max(global_max_profit, dp[openings.back().job_id]);

    openings.pop_back();
  }

  // Ao final da função, global_max_profit será o valor máximo do vetor dp.
  return global_max_profit;
}

#include <iostream>

// Para fins de testabilidade; caso desnecessária, pode ser ignorada.
int main(int argc, char *argv[]) {
  int n;

  if (argc == 1) {
    cin >> n;
  } else {
    n = stoi(argv[1]);
  }

  if (n < 1 || n > 5 * 1e4) {
    cerr << "Erro: o tamanho dos vetores deve estar no intervalo [1, 5*10^4] "
            "(agora ele é "
         << n << ")." << endl;
    return EXIT_FAILURE;
  }

  vector<int> startTime(n), endTime(n), profit(n);

  if (argc == 1) {
    for (int n_i = 0; n_i < n; ++n_i) {
      cin >> startTime[n_i];
    }
    for (int n_i = 0; n_i < n; ++n_i) {
      cin >> endTime[n_i];
    }
    for (int n_i = 0; n_i < n; ++n_i) {
      cin >> profit[n_i];
    }
  } else {
    if (argc != 2 + 3 * n) {
      cerr << "Erro: número de argumentos incoerente na linha de comandos (são "
              "requisitados "
           << 1 + 3 * n << ", mas " << argc - 1 << " foram fornecidos)."
           << endl;
      return EXIT_FAILURE;
    }

    for (int n_i = 0; n_i < n; ++n_i) {
      startTime[n_i] = stoi(argv[2 + n_i]);
    }
    for (int n_i = 0; n_i < n; ++n_i) {
      endTime[n_i] = stoi(argv[2 + n + n_i]);
    }
    for (int n_i = 0; n_i < n; ++n_i) {
      profit[n_i] = stoi(argv[2 + 2 * n + n_i]);
    }
  }

  for (int n_i = 0; n_i < n; ++n_i) {
    if (startTime[n_i] < 1 || endTime[n_i] < 1 || startTime[n_i] > 1e9 ||
        endTime[n_i] > 1e9) {
      cerr << "Erro: os vetores de tempo devem ter valores no intervalo [1, "
              "10^9] (o "
           << n_i + 1 << "º job quebra essa regra)." << endl;
      return EXIT_FAILURE;
    }

    if (profit[n_i] < 1 || profit[n_i] > 1e4) {
      cerr << "Erro: o vetor de lucro deve ter valores no intervalo [1, 10^4] "
              "(o "
           << n_i + 1 << "º job quebra essa regra)." << endl;
      return EXIT_FAILURE;
    }

    if (startTime[n_i] >= endTime[n_i]) {
      cerr << "Erro: o início de um job deve preceder o seu término (o "
           << n_i + 1 << "º job quebra essa regra)." << endl;
      return EXIT_FAILURE;
    }
  }

  cout << jobScheduling(startTime, endTime, profit) << endl;

  return EXIT_SUCCESS;
}
