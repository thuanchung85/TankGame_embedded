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

STEP 2: Declare the C-Compatible Task Handler Interface (task_list.h)
Because the Active Kernel core is compiled using standard C calling conventions, but game modules may be implemented using C++ source properties, explicit Linkage Declarations are required.

At the bottom of task_list.h, enclose the handler function prototypes inside an #ifdef __cplusplus / extern "C" guard wrapper to avoid symbol name mangling during compilation links.

#ifdef __cplusplus
extern "C" {
#endif

extern void task_ground_handle(ak_msg_t *msg);
extern void task_tree_handle(ak_msg_t *msg);   
extern void task_building_handle(ak_msg_t *msg);   
extern void task_mountain_handle(ak_msg_t *msg);   
extern void task_tank_handle(ak_msg_t *msg);   
extern void task_cannon_bullet_handle(ak_msg_t *msg);   
extern void task_enemy_handle(ak_msg_t *msg);   
extern void task_minigun_bullet_handle(ak_msg_t *msg);   
extern void task_score_handle(ak_msg_t *msg);   
extern void task_boss_handle(ak_msg_t *msg);   
extern void task_boss2_handle(ak_msg_t *msg);   
extern void task_trap_handle(ak_msg_t *msg);   

#ifdef __cplusplus
}
#endif

STEP 3: Bind Array Mapping inside the Master Dispatch Table (task_list.cpp)
Open task_list.cpp and locate the immutable scheduling dispatch matrix named const task_t app_task_table[]. Every record entry instantiated within this table binds the Task ID, Execution Priority Level, and the targeted Callback Handler Entry Point.

Insert the Tank Game definitions array segment exactly prior to the terminal sentinel element {AK_TASK_EOT_ID, TASK_PRI_LEVEL_0, (pf_task)0}:

const task_t app_task_table[] = {
    /* ... Pre-existing Core App Tasks Rows ... */
    {AC_TASK_DISPLAY_ID         ,   TASK_PRI_LEVEL_4    ,   task_display        },

    /*************************************************************************/
    /* TANK GAME TASK */
    /*************************************************************************/
    {TG_GROUND_TASK_ID          ,  TASK_PRI_LEVEL_4    ,  task_ground_handle },
    {TG_TREE_TASK_ID            ,  TASK_PRI_LEVEL_4    ,  task_tree_handle  }, 
    {TG_BUILDING_TASK_ID        ,  TASK_PRI_LEVEL_4    ,  task_building_handle  }, 
    {TG_MOUNTAIN_TASK_ID        ,  TASK_PRI_LEVEL_4    ,  task_mountain_handle  }, 
    {TG_TANK_TASK_ID            ,  TASK_PRI_LEVEL_4    ,  task_tank_handle  }, 
    {TG_CANNON_BULLET_TASK_ID   ,  TASK_PRI_LEVEL_4    ,  task_cannon_bullet_handle  }, 
    {TG_ENEMY_TASK_ID           ,  TASK_PRI_LEVEL_4    ,  task_enemy_handle  }, 
    {TG_MINIGUN_BULLET_TASK_ID  ,  TASK_PRI_LEVEL_4    ,  task_minigun_bullet_handle  }, 
    {TG_SCORE_TASK_ID           ,  TASK_PRI_LEVEL_4    ,  task_score_handle  }, 
    {TG_BOSS_TASK_ID            ,  TASK_PRI_LEVEL_4    ,  task_boss_handle  }, 
    {TG_BOSS2_TASK_ID           ,  TASK_PRI_LEVEL_4    ,  task_boss2_handle  }, 
    {TG_TRAP_TASK_ID            ,  TASK_PRI_LEVEL_4    ,  task_trap_handle  }, 

    /*************************************************************************/
    /* END OF TABLE */
    /*************************************************************************/
    {AK_TASK_EOT_ID             ,   TASK_PRI_LEVEL_0    ,   (pf_task)0          }
};