TECHNICAL GUIDELINES: ACTIVE KERNEL TASK REGISTRATION PROTOCOL

- Target Configuration Files: task_list.h, task_list.cpp
- System Sub-Section: /*************************************************************************/
                      /* TANK GAME TASK */
                      /*************************************************************************/
- Architecture Framework: Active Kernel (AK) Event-Driven Task Scheduler

1. Architectural Overview
In the Active Kernel (AK) environment, every standalone subsystem (e.g., player tank, enemies, dynamic scrolling layers) operates as an isolated execution thread managed by an asynchronous event loop. For a custom subsystem to receive, parse, and process kernel message vectors (`ak_msg_t`), it must be explicitly declared and registered across two synchronized definition layers:
1. Architectural ID Enumeration Block (`task_list.h`)
2. Function Signature Linkage Block (`task_list.h`)
3. Global Scheduling Allocation Table (`task_list.cpp`)

---

2. Step-by-Step Registration Workflow

STEP 1: Define the Unique Identifier Enum (task_list.h)
Every unique task requires a dedicated Task ID. Open `task_list.h` and locate the anonymous task ID enumeration cluster (`enum`). Insert the specific Tank Game identifiers directly before the End-of-Table anchor (`AK_TASK_EOT_ID`):

```c
enum {
    /* ... Pre-existing System and Application Task IDs ... */
    AC_TASK_DISPLAY_ID,

    /*************************************************************************/
    /* TANK GAME TASK      */
    /*************************************************************************/
    TG_GROUND_TASK_ID,
    TG_TREE_TASK_ID,
    TG_BUILDING_TASK_ID,
    TG_MOUNTAIN_TASK_ID,
    TG_TANK_TASK_ID,
    TG_CANNON_BULLET_TASK_ID,
    TG_ENEMY_TASK_ID,
    TG_MINIGUN_BULLET_TASK_ID,
    TG_SCORE_TASK_ID,
    TG_BOSS_TASK_ID,
    TG_BOSS2_TASK_ID,
    TG_TRAP_TASK_ID,

    /* EOT task ID (End of Table) */
    AK_TASK_EOT_ID, // MUST remain at the absolute bottom
};