#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/util.h>
#include <string.h>

// Arquivo de controle do shell e seus "comandos"
//  Estrutura auxiliar para passar o shell para os callbacks
struct thread_cb_ctx
{
    const struct shell *shell;
};

// Callback para listar threads
static void thread_info_cb(const struct k_thread *thread, void *user_data)
{
    // recuperar o contexto
    struct thread_cb_ctx *ctx = (struct thread_cb_ctx *)user_data;
    const struct shell *shell = ctx->shell;

    const char *name = k_thread_name_get((struct k_thread *)thread); // recebe o nome da thread

    size_t unused;
    int ret = k_thread_stack_space_get((struct k_thread *)thread, &unused); // obtem o espaço livre da pilha

    // printa no shell as informações da thread
    shell_print(shell,
                "Thread: %-16s | Prioridade: %2d | Estado: %d | Stack Livre: %u bytes",
                name ? name : "SEM_NOME",
                thread->base.prio,
                thread->base.thread_state,
                ret == 0 ? (unsigned int)unused : 0);
}

// Comando: mostrar threads instaladas
static int cmd_threads(const struct shell *shell, size_t argc, char **argv)
{
    // ignora argumentos não usados
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    // printa no shell as threads
    shell_print(shell, "\n=== Lista de Threads ===\n");

    struct thread_cb_ctx ctx = {.shell = shell}; // prepara o contexto que será enviado para o callback

    // percorre as threads existentes
    k_thread_foreach(thread_info_cb, &ctx);

    return 0;
}

// Comando: mostrar heap livre
static int cmd_heap(const struct shell *shell, size_t argc, char **argv)
{
    // ignora argumentos não usados
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    // printa no shell as informações da heap
    shell_print(shell, "\n=== Informações de HEAP ===");
    shell_print(shell, "Tamanho configurado: %d bytes", K_HEAP_MEM_POOL_SIZE);

    // Tenta alocar e liberar para verificar disponibilidade
    void *test_ptr = k_malloc(64);
    // se alocou memoria
    if (test_ptr != NULL)
    {
        k_free(test_ptr); // libera
        shell_print(shell, "Status: Heap operacional");
    }
    else
    {
        shell_print(shell, "Status: Heap pode estar cheio");
    }

    return 0;
}

// Callback para runtime
static void runtime_cb(const struct k_thread *thread, void *user_data)
{
    // recuperar o contexto
    struct thread_cb_ctx *ctx = (struct thread_cb_ctx *)user_data;
    const struct shell *shell = ctx->shell;

    k_thread_runtime_stats_t stats;
    int ret = k_thread_runtime_stats_get((struct k_thread *)thread, &stats); // obtem estatísticas da thread

    // se tem estatisticas
    if (ret == 0)
    {
        const char *name = k_thread_name_get((struct k_thread *)thread); // recebe o nome da thread

        // Converter ciclos para microsegundos
        uint64_t runtime_us = k_cyc_to_us_ceil64(stats.execution_cycles);

        // printa no shell as estatisticas
        shell_print(shell,
                    "%-16s | Runtime: %10llu us | Ciclos: %llu",
                    name ? name : "SEM_NOME",
                    runtime_us,
                    stats.execution_cycles);
    }
}

// Comando: runtime de threads
static int cmd_runtime(const struct shell *shell, size_t argc, char **argv)
{
    // ignora argumentos não usados
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    // printa no shell o runtime das threads
    shell_print(shell, "\n=== Runtime das Threads ===\n");

    // prepara o contexto que será enviado para o callback
    struct thread_cb_ctx ctx = {.shell = shell};

    // percorre as threads existentes
    k_thread_foreach(runtime_cb, &ctx);

    return 0;
}

// Callback para informações de tempo real
static void rtinfo_cb(const struct k_thread *thread, void *user_data)
{
    // recuperar o contexto
    struct thread_cb_ctx *ctx = (struct thread_cb_ctx *)user_data;
    const struct shell *shell = ctx->shell;

    const char *name = k_thread_name_get((struct k_thread *)thread); // recebe o nome da thread

    // Estado da thread
    const char *state_str;
    uint8_t state = thread->base.thread_state; // pega o estado atual da thread

    // de acordo com o estado da thread ele seta o state_str
    switch (state)
    {
    case 0:
        state_str = "READY";
        break;
    case 1:
        state_str = "SUSPENDED";
        break;
    case 2:
        state_str = "PENDING";
        break;
    case 4:
        state_str = "DEAD";
        break;
    case 8:
        state_str = "QUEUED";
        break;
    default:
        state_str = "UNKNOWN";
        break;
    }

    // Obter uso de stack
    size_t unused;
    int ret = k_thread_stack_space_get((struct k_thread *)thread, &unused); // obtem o espaço livre da pilha

    // printa no shell as informações do tempo real
    shell_print(shell,
                "%-16s | Prioridade: %2d | Preempção: %d | Estado: %-10s | Stack: %4u bytes",
                name ? name : "SEM_NOME",
                thread->base.prio,
                thread->base.preempt,
                state_str,
                ret == 0 ? (unsigned int)unused : 0);
}

// Comando: informações de tarefas em tempo real
static int cmd_rtinfo(const struct shell *shell, size_t argc, char **argv)
{
    // ignora argumentos não usados
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    // printa as informações no shell
    shell_print(shell, "\n=== Informações de Tarefas Tempo Real ===\n");

    // prepara o contexto que será enviado para o callback
    struct thread_cb_ctx ctx = {.shell = shell};

    // percorre as threads existentes
    k_thread_foreach(rtinfo_cb, &ctx);

    return 0;
}

// Registrar comandos no shell
SHELL_CMD_REGISTER(tasks, NULL, "Lista todas as threads do sistema", cmd_threads);
SHELL_CMD_REGISTER(heapinfo, NULL, "Mostra informações de heap", cmd_heap);
SHELL_CMD_REGISTER(runtime, NULL, "Mostra tempo de execução das threads", cmd_runtime);
SHELL_CMD_REGISTER(rtinfo, NULL, "Informações detalhadas de tarefas RT", cmd_rtinfo);