#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <time.h>

struct World;

typedef struct
{
    int x;
    int y;
}
vec2;

typedef struct
{
    vec2 position;
    vec2 size;
}
Object;

typedef struct SnakePart
{
    Object object;
    vec2 moveDir;
    struct SnakePart* next;
    int New;
    int count;
}
SnakePart;

typedef struct
{
    Object object;
    double time_of_life;
    void (*spawn)(struct World* world, double time_of_life);
    void (*spawn2)(struct World* world, double time_of_life);
}
Food;

typedef struct World
{
    vec2 windowSize;
    int grid;
    double food_life_on_reset;
    Object walls[4];
    Object obstacles[4]; //dla modu level
    SnakePart* head;
    Food food;
    int GameOver;
    int NewLevel;
}
World;

int initAllegro();
World* initWorld(vec2 windowSize, int grid);
World* initWorld2(vec2 windowSize, int grid);
World* initWorld3(vec2 windowSize, int grid);
World* initWorld4(vec2 windowSize, int grid);
World* initMenu(vec2 windowSize); //
void procInput(ALLEGRO_EVENT_QUEUE* eventQueue, World* world, int* quit);
void procInput2(ALLEGRO_EVENT_QUEUE* eventQueue, World* world, int* quit);
void menuInput(ALLEGRO_EVENT_QUEUE* eventQueue, int* quit2);//
void update(World* world, double dt);
void update2(World* world, double dt);
void render(World* world);
void render2(World* world);
void rend_Menu();
void deleteWorld(World* world);

int isCollision(Object *e1, Object *e2);
void move(SnakePart* head, int grid);
void move2(SnakePart* head, int grid);
void spawn(World* world, double time_of_life);
void spawn2(World* world, double time_of_life);
void drawObject(Object* object, ALLEGRO_COLOR color);
void resetSnake(SnakePart* head);
int moveFlag = 0;
int u = 0;
int worldtype = 0;
int level = 0;
int speed = 0;
double dt = 0.2;
int highscore = 0;
int main()
{
    if(!initAllegro())
        return -1;

    srand(time(NULL));
    const vec2 windowSize = {800, 800};
    const int grid = 20;

    double dt = 0.15;
    ALLEGRO_DISPLAY* window = al_create_display(windowSize.x, windowSize.y);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    al_register_event_source(eventQueue, al_get_display_event_source(window));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    FILE *fp;
    fp = fopen("highscore", "r");
    if(fp == NULL)
    {
        fp = fopen("highscore", "w");
        fprintf(fp, "%d", highscore);
        fclose(fp);
    }
    else
    {
        fscanf(fp, "%d", &highscore);
        fclose(fp);
    }
    int quit = false;
    int quit2 = false;
    double currentTime = al_get_time();
    double addTime = 0.0;

    World* menu = initMenu(windowSize);

    do
    {
        worldtype = 0;
        while(worldtype == 0 && !quit2)
        {
            menuInput(eventQueue, &quit2);
            rend_Menu(menu);
        }
        if(worldtype == 1)
        {
            u = 0;
            World* world = initWorld(windowSize, grid);
            quit = false;
            while (!quit)
            {
                procInput(eventQueue, world, &quit);

                double newTime = al_get_time();
                double frameTime = newTime - currentTime;
                addTime += frameTime;
                currentTime = newTime;

                while (addTime >= dt)
                {
                    if(world->GameOver == 0)
                        update(world, dt);
                    addTime -= dt;
                }
                render(world);
            }
            deleteWorld(world);
            worldtype = 0;

        }
        else if(worldtype == 2)
        {
            quit = false;
            while (!quit)
            {
                level++;
                if(level%3 == 1)
                {
                    u = 0;
                    World* world2 = initWorld2(windowSize, grid);
                    while (u < 4)
                    {
                        procInput2(eventQueue, world2, &quit);

                        double newTime = al_get_time();
                        double frameTime = newTime - currentTime;
                        addTime += frameTime;
                        currentTime = newTime;

                        while (addTime >= dt)
                        {
                            if(world2->GameOver == 0)
                                update2(world2, dt);
                            addTime -= dt;
                        }
                        render2(world2);
                    }

                    deleteWorld(world2);
                    u = 0;
                }
                else if(level%3 == 2)
                {
                    World* world3 = initWorld3(windowSize, grid);
                    while (u < 4)
                    {
                        procInput2(eventQueue, world3, &quit);

                        double newTime = al_get_time();
                        double frameTime = newTime - currentTime;
                        addTime += frameTime;
                        currentTime = newTime;

                        while (addTime >= dt)
                        {
                            if(world3->GameOver == 0)
                                update2(world3, dt);
                            addTime -= dt;
                        }
                        render2(world3);
                    }
                    deleteWorld(world3);
                    u = 0;
                }
                else if(level%3 == 0)
                {
                    World* world4 = initWorld4(windowSize, grid);
                    while (u < 4)
                    {
                        procInput2(eventQueue, world4, &quit);

                        double newTime = al_get_time();
                        double frameTime = newTime - currentTime;
                        addTime += frameTime;
                        currentTime = newTime;

                        while (addTime >= dt)
                        {
                            if(world4->GameOver == 0)
                                update2(world4, dt);
                            addTime -= dt;
                        }
                        render2(world4);
                    }
                    deleteWorld(world4);
                    u = 0;
                }
                dt /= 1.2;
            }
        }
    }
    while (!quit2);
    deleteWorld(menu);
    al_destroy_event_queue(eventQueue);
    al_destroy_display(window);
    return 0;
}

//**********************************************************************************************************************
int initAllegro()
{
    if(!(al_init() && al_install_keyboard() && al_init_primitives_addon()))
        return 0;
    al_init_font_addon();
    return 1;
}
//**********************************************************************************************************************
World* initWorld(vec2 windowSize, int grid)
{
   World* world = malloc(sizeof(World));

   world -> windowSize = windowSize;
   world -> grid = grid;
   world -> food_life_on_reset = 60.0;

   world -> walls[0] = (Object){{0, 0}, {grid, windowSize.y}};
   world -> walls[1] = (Object){{windowSize.x - grid, 0}, {grid, windowSize.y}};
   world -> walls[2] = (Object){{0, 0}, {windowSize.x, grid}};
   world -> walls[3] = (Object){{0, windowSize.y - grid}, {windowSize.x, grid}};

   world ->food.spawn = &spawn;
   world -> food.object.size = (vec2){grid, grid};
   world -> food.spawn(world, world -> food_life_on_reset); // może crashowac
   world -> GameOver = 0;

   world ->head = malloc(sizeof(SnakePart));
   world ->head ->object = (Object){{windowSize.x/2, windowSize.y/2}, {grid, grid}};
   world ->head ->moveDir = (vec2){0, -1};
   world ->head ->New = 0;
   world ->head ->next = 0;

   return world;
}
//**********************************************************************************************************************
World *initMenu(vec2 windowSize)
{
    World* world = malloc(sizeof(World));

    world->windowSize = windowSize;

    return world;
}

//*********************************************************************************************************
void update(World* world, double dt)
{
    FILE *fp;
    moveFlag = 0;
    move(world -> head, world -> grid);
    world -> food.time_of_life -= dt;

    if(world -> food.time_of_life <= 0)
        world -> food.spawn(world, world -> food_life_on_reset);
    if(isCollision(&world -> head -> object, &world -> food.object))
    {
        SnakePart* part = world -> head;
        int count = 1;
        while (part -> next != 0)
        {
            ++count;
            part = part -> next;
        }
        part -> next = malloc(sizeof(SnakePart));
        part -> next -> next = 0;
        part -> next -> count = count;
        part -> next -> New = 1;
        u++;
        part -> next -> object = world -> food.object;
        part -> next -> moveDir = (vec2){0, 0};

        world -> food.spawn(world, world -> food_life_on_reset);
    }

    for(unsigned int i = 0; i < 4; ++i)
    {
        if(isCollision(&world -> head -> object, &world -> walls[i]))
        {
            world ->GameOver = 1;
            if(u > highscore)
            {
                highscore = u;
                fp = fopen("highscore", "w");
                if(fp == NULL)
                    exit(1);
                fprintf(fp, "%d", highscore);
                fclose(fp);
            }
            u = 0;
            return;
        }
    }

    SnakePart* part = world ->head->next;
    while (part != 0)
    {
        if(part ->New == 0)
        {
            if(isCollision(&part ->object, &world ->head ->object))
            {
                world ->GameOver = 1;
                if(u > highscore)
                {
                    highscore = u;
                    fp = fopen("highscore", "w");
                    if(fp == NULL)
                        exit(1);
                    fprintf(fp, "%d", highscore);
                    fclose(fp);
                }
                u = 0;
                return;
            }
        }
        part = part ->next;
    }
}
//**********************************************************************************************************************
void update2(World *world, double dt)
{
    moveFlag = 0;
    move2(world -> head, world -> grid);
    world -> food.time_of_life -= dt;

    if(world -> food.time_of_life <= 0)
        world -> food.spawn2(world, world -> food_life_on_reset);
    if(isCollision(&world -> head -> object, &world -> food.object))
    {
        SnakePart* part = world -> head;
        int count = 1;
        while (part -> next != 0)
        {
            ++count;
            part = part -> next;
        }
        part -> next = malloc(sizeof(SnakePart));
        part -> next -> next = 0;
        part -> next -> count = count;
        part -> next -> New = 1;
        u++;
        part -> next -> object = world -> food.object;
        part -> next -> moveDir = (vec2){0, 0};

        world -> food.spawn2(world, world -> food_life_on_reset);
    }

    for(unsigned int i = 0; i < 4; ++i)
    {
        if(isCollision(&world -> head -> object, &world -> walls[i]))
        {
            world ->GameOver = 1;
            u = 0;
            level = 1;
            return;
        }
    }

    for(int j = 0; j < 4; ++j)
    {
        if (isCollision(&world -> head ->object, &world ->obstacles[j]))
        {
            world ->GameOver = 1;
            u = 0;
            level = 1;
            return;
        }
    }

    SnakePart* part = world ->head->next;
    while (part != 0)
    {
        if(part ->New == 0)
        {
            if(isCollision(&part ->object, &world ->head ->object))
            {
                world ->GameOver = 1;
                u = 0;
                level = 1;
                return;
            }
        }
        part = part ->next;
    }
}

//**********************************************************************************************************************
void rend_Menu()
{
    typedef ALLEGRO_COLOR COLOR;
    al_clear_to_color((COLOR){0, 0, 0, 0});
    ALLEGRO_FONT*font = al_create_builtin_font();
    al_draw_text(font, (COLOR){255, 255, 255, 0}, 400, 400 - 100,ALLEGRO_ALIGN_CENTER,
                 "PRESS 1 TO PLAY SURVIVAL MODE");
    al_draw_text(font, (COLOR){255, 255, 255, 0}, 400, 400 - 50,ALLEGRO_ALIGN_CENTRE,
                 "PRESS 2 TO PLAY LEVEL MODE");
    al_draw_text(font, (COLOR){255, 255, 255, 0}, 400, 400,ALLEGRO_ALIGN_CENTRE,
                 "PRESS ESC TO QUIT THE GAME");
    al_flip_display();
}

//*********************************************************************************************************
void render(World *world)
{
    typedef ALLEGRO_COLOR COLOR;
    al_clear_to_color((COLOR){0, 0, 0, 0});

    for(int i = 0; i < 4; ++i)
        drawObject(&world ->walls[i], (COLOR){255,0,120, 0});

    SnakePart* part = world->head;
    while (part != 0)
    {
        COLOR color;
        if(part ->New == 1)
            color = (COLOR){120, 120, 120, 0};
        else if (part == world -> head)
            color = (COLOR){255, 120, 0, 0};
        else
            color = (COLOR){0, 255, 0, 0};
        drawObject(&part ->object, color);
        part = part ->next;
    }

    drawObject(&world ->food.object, (COLOR){150, 0, 120, 0});

    ALLEGRO_FONT*font = al_create_builtin_font();
    char SCORE1[20] = "Your Score: ";
    char HIGHSCORE[20] = "Your Highscore: ";
    sprintf(SCORE1 + 12, "%d", u);
    sprintf(HIGHSCORE + 16, "%d", highscore);

    al_draw_text(font, (COLOR){255, 0, 0, 0}, world ->windowSize.x/2, world ->windowSize.y/2 + 300, ALLEGRO_ALIGN_CENTER,SCORE1);
    al_draw_text(font, (COLOR){255, 0, 0, 0}, world ->windowSize.x/2, world ->windowSize.y/2 + 350, ALLEGRO_ALIGN_CENTER,HIGHSCORE);
    if(world ->GameOver)
    {
        ALLEGRO_FONT*font = al_create_builtin_font();
        al_draw_text(font, (COLOR){255, 0, 0, 0}, world ->windowSize.x / 2, world ->windowSize.y / 2,ALLEGRO_ALIGN_CENTRE,
                     "GAME OVER, press R to restart, ESC to go back to main menu.");
        char SCORE[20] = "Your Score: ";
        int x = 0;
        SnakePart* part = world ->head ->next;
        while (part)
        {
            ++x;                    //gdzieś tutaj dodaj highscore'a
            part = part ->next;
        }
        sprintf(SCORE + 12, "%d", x);

        al_draw_text(font, (COLOR){255, 0, 0, 0}, world ->windowSize.x/2, world ->windowSize.y/2 + 50, ALLEGRO_ALIGN_CENTER,SCORE);

        al_destroy_font(font);
    }
    al_flip_display();
}
//**********************************************************************************************************************
void render2(World *world)
{
    typedef ALLEGRO_COLOR COLOR;
    al_clear_to_color((COLOR){0, 0, 0, 0});

    for(int i = 0; i < 4; ++i)
        drawObject(&world ->walls[i], (COLOR){255,0,120, 0});

    for(int j = 0; j < 4; ++j)
        drawObject(&world ->obstacles[j], (COLOR){255,0,120, 0});

    SnakePart* part = world->head;
    while (part != 0)
    {
        COLOR color;
        if(part ->New == 1)
            color = (COLOR){120, 120, 120, 0};
        else if (part == world -> head)
            color = (COLOR){255, 120, 0, 0};
        else
            color = (COLOR){0, 255, 0, 0};
        drawObject(&part ->object, color);
        part = part ->next;
    }

    drawObject(&world ->food.object, (COLOR){150, 0, 120, 0});

    ALLEGRO_FONT*font = al_create_builtin_font();
    char SCORE1[20] = "Your Score: ";
    sprintf(SCORE1 + 12, "%d", u);

    al_draw_text(font, (COLOR){255, 0, 0, 0}, world ->windowSize.x/2, world ->windowSize.y/2 + 300, ALLEGRO_ALIGN_CENTER,SCORE1);

    if(world ->GameOver)
    {
        ALLEGRO_FONT*font = al_create_builtin_font();
        al_draw_text(font, (COLOR){255, 0, 0, 0}, world ->windowSize.x / 2, world ->windowSize.y / 2,ALLEGRO_ALIGN_CENTRE,
                     "GAME OVER, press R to restart, ESC to go back to main menu.");
        char SCORE[20] = "Your Score: ";
        int x = 0;
        SnakePart* part = world ->head ->next;
        while (part)
        {
            ++x;
            part = part ->next;
        }
        sprintf(SCORE + 12, "%d", x);

        al_draw_text(font, (COLOR){255, 0, 0, 0}, world ->windowSize.x/2, world ->windowSize.y/2 + 50, ALLEGRO_ALIGN_CENTER,SCORE);

        al_destroy_font(font);
    }
    al_flip_display();
}

//**********************************************************************************************************************
void move(SnakePart *head, int grid)
{
    int count = 0;
    SnakePart* part = head;
    while (part != 0)
    {
        if(part ->New)
        {
            --part ->count;
            if(part ->count <=0)
            {
                part ->New = 0;
            }
        }
        else
        {
             part ->object.position.x += part ->moveDir.x *grid;
             part ->object.position.y += part ->moveDir.y *grid;
        }
        part = part ->next;
        ++count;
    }
    int count_2 = count - 2;
    for(int i = 0; i < count - 1; ++i)
    {
        SnakePart* part = head ->next;
        SnakePart* previous_part = head;
        for(int j = 0; j < count_2; j++)
        {
            previous_part = part;
            part = part ->next;
        }
        --count_2;
        part -> moveDir = previous_part ->moveDir;
    }
}
//**********************************************************************************************************************
void move2(SnakePart *head, int grid)
{
    int count = 0;
    SnakePart* part = head;
    while (part != 0)
    {
        if(part ->New)
        {
            --part ->count;
            if(part ->count <=0)
            {
                part ->New = 0;
            }
        }
        else
        {
             part ->object.position.x += part ->moveDir.x *grid;
             part ->object.position.y += part ->moveDir.y *grid;
        }
        part = part ->next;
        ++count;
    }
    int count_2 = count - 2;
    for(int i = 0; i < count - 1; ++i)
    {
        SnakePart* part = head ->next;
        SnakePart* previous_part = head;
        for(int j = 0; j < count_2; j++)
        {
            previous_part = part;
            part = part ->next;
        }
        --count_2;
        part -> moveDir = previous_part ->moveDir;
    }
}

//**********************************************************************************************************************
void spawn(World *world, double time_of_life)
{
    world ->food.time_of_life = time_of_life;
    int gridsX = world ->windowSize.x / world ->grid;
    int gridsY = world ->windowSize.y / world ->grid;

    while (true)
    {
        vec2 newPosition = {(rand() % gridsX)*world ->grid,
                            (rand() % gridsY)*world ->grid};
        world ->food.object.position = newPosition;

        int flag1 = 0;
        for(unsigned i = 0; i < 4; ++i)
            if(isCollision(&world ->walls[i], &world ->food.object))
                flag1 = 1;
        int flag2 = 0;
        SnakePart* part = world ->head;
        while(part != 0)
        {
            if(isCollision(&part ->object, &world ->food.object))
            {
                flag2 = true;
                break;
            }
            part = part ->next;
        }
        if(!flag2 && !flag1)
            return;
    }
}
//**********************************************************************************************************************
void spawn2(World *world, double time_of_life)
{
    world ->food.time_of_life = time_of_life;
    int gridsX = world ->windowSize.x / world ->grid;
    int gridsY = world ->windowSize.y / world ->grid;

    while (true)
    {
        vec2 newPosition = {(rand() % gridsX)*world ->grid,
                            (rand() % gridsY)*world ->grid};
        world ->food.object.position = newPosition;

        int flag1 = 0;
        for(unsigned i = 0; i < 4; ++i)
            if(isCollision(&world ->walls[i], &world ->food.object))
                flag1 = 1;
        int flag2 = 0;
        SnakePart* part = world ->head;
        while(part != 0)
        {
            if(isCollision(&part ->object, &world ->food.object))
            {
                flag2 = true;
                break;
            }
            part = part ->next;
        }
        int flag3 = 0;
        for (unsigned int j = 0; j < 4; ++j)
            if(isCollision(&world ->obstacles[j], &world ->food.object))
                flag3 = 1;
        if(!flag2 && !flag1 && !flag3)
            return;
    }
}
//**********************************************************************************************************************
void drawObject(Object *object, ALLEGRO_COLOR color)
{
    al_draw_filled_rectangle(object ->position.x + 1, object ->position.y + 1,
                             object ->position.x + object ->size.x - 1, object ->position.y + object -> size.y - 1,
                             color);
}
//**********************************************************************************************************************
void resetSnake(SnakePart *head)
{
    SnakePart* part = head ->next;
    head ->next = 0;
    while(part != 0)
    {
        SnakePart* tempo = part ->next;
        free(part);
        part = tempo;
    }
}
//**********************************************************************************************************************
void deleteWorld(World *world)
{
    SnakePart* part = world ->head;
    while (part != 0)
    {
        SnakePart* tempo = part->next;
        free(part);
        part = tempo;
    }
    free(world);
}
//**********************************************************************************************************************
int isCollision(Object *e1, Object *e2)
{
    int x = e1 ->position.x < e2 ->position.x + e2 ->size.x
            && e2 ->position.x < e1 ->position.x + e1 ->size.x;

    int y = e1 ->position.y < e2 ->position.y + e2 ->size.y
            && e2 ->position.y < e1->position.y + e1->size.y;
    return x && y;
}
//**********************************************************************************************************************
void procInput(ALLEGRO_EVENT_QUEUE *eventQueue, World *world, int *quit)
{
    ALLEGRO_EVENT event;
    while (al_get_next_event(eventQueue, &event))
    {
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            *quit = true;
        if(!world ->GameOver)
        {
            if(event.type == ALLEGRO_EVENT_KEY_DOWN && !moveFlag)
            {
                switch(event.keyboard.keycode)
                {
                case ALLEGRO_KEY_UP:
                    if(world ->head ->moveDir.y !=0)
                        break;
                        world ->head ->moveDir.y = -1;
                    world ->head ->moveDir.x = 0;
                    moveFlag = 1;
                    return;

                case ALLEGRO_KEY_DOWN:
                    if(world ->head ->moveDir.y != 0)
                        break;
                        world ->head ->moveDir.y = 1;
                    world ->head ->moveDir.x = 0;
                    moveFlag = 1;
                    return;

                case ALLEGRO_KEY_LEFT:
                    if(world ->head ->moveDir.x != 0)
                        break;
                        world ->head ->moveDir.x = -1;
                    world ->head ->moveDir.y = 0;
                    moveFlag = 1;
                    return;

                case ALLEGRO_KEY_RIGHT:
                    if(world ->head ->moveDir.x != 0)
                        break;
                        world ->head ->moveDir.x = 1;
                    world ->head ->moveDir.y = 0;
                    moveFlag = 1;
                    return;

                 case ALLEGRO_KEY_ESCAPE: *quit = true; return;
                default:;
                }
            }
        }
        else
        {
            if(event.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch (event.keyboard.keycode)
                {
                case ALLEGRO_KEY_R: world ->GameOver = 0;
                    resetSnake(world ->head);
                    world ->head ->object.position = (vec2){world ->windowSize.x/2, world ->windowSize.x/2};
                    world ->head ->moveDir = (vec2){0, -1};
                    return;

                case ALLEGRO_KEY_ESCAPE: *quit = true; return;
                default:;
                }
            }
        }
    }
}
//********************************************************************************************************************
void procInput2(ALLEGRO_EVENT_QUEUE *eventQueue, World *world, int *quit)
{
    ALLEGRO_EVENT event;
    while (al_get_next_event(eventQueue, &event))
    {
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            *quit = true;
        if(!world ->GameOver)
        {
            if(event.type == ALLEGRO_EVENT_KEY_DOWN && !moveFlag)
            {
                switch(event.keyboard.keycode)
                {
                case ALLEGRO_KEY_UP:
                    if(world ->head ->moveDir.y !=0)
                        break;
                        world ->head ->moveDir.y = -1;
                    world ->head ->moveDir.x = 0;
                    moveFlag = 1;
                    return;

                case ALLEGRO_KEY_DOWN:
                    if(world ->head ->moveDir.y != 0)
                        break;
                        world ->head ->moveDir.y = 1;
                    world ->head ->moveDir.x = 0;
                    moveFlag = 1;
                    return;

                case ALLEGRO_KEY_LEFT:
                    if(world ->head ->moveDir.x != 0)
                        break;
                        world ->head ->moveDir.x = -1;
                    world ->head ->moveDir.y = 0;
                    moveFlag = 1;
                    return;

                case ALLEGRO_KEY_RIGHT:
                    if(world ->head ->moveDir.x != 0)
                        break;
                        world ->head ->moveDir.x = 1;
                    world ->head ->moveDir.y = 0;
                    moveFlag = 1;
                    return;

                 case ALLEGRO_KEY_ESCAPE: *quit = true; return;
                default:;
                }
            }
        }
        else
        {
            if(event.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch (event.keyboard.keycode)
                {
                case ALLEGRO_KEY_R: world ->GameOver = 0;
                    u = 4;
                    level = 0;
                    dt = 0.2;
                    return;

                case ALLEGRO_KEY_ESCAPE: *quit = true;
                    u = 4;
                    level = 0;
                    return;
                default:;
                }
            }
        }
    }
}

//******************************************************************************************************************
void menuInput(ALLEGRO_EVENT_QUEUE *eventQueue, int *quit2)
{
    ALLEGRO_EVENT event;
    while (al_get_next_event(eventQueue, &event))
    {
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            *quit2 = true;
        if(event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(event.keyboard.keycode)
            {
            case ALLEGRO_KEY_1:
                worldtype = 1;
                return;
            case ALLEGRO_KEY_2:
                worldtype = 2;
                return;
            case ALLEGRO_KEY_ESCAPE: *quit2 = true; exit(1); return;
            default:;

            }
         }
     }
}
//**********************************************************************************************************************
World* initWorld2(vec2 windowSize, int grid)
{
    World* world = malloc(sizeof(World));

    world -> windowSize = windowSize;
    world -> grid = grid;
    world -> food_life_on_reset = 60.0;

    world -> walls[0] = (Object){{0, 0}, {grid, windowSize.y}};
    world -> walls[1] = (Object){{windowSize.x - grid, 0}, {grid, windowSize.y}};
    world -> walls[2] = (Object){{0, 0}, {windowSize.x, grid}};
    world -> walls[3] = (Object){{0, windowSize.y - grid}, {windowSize.x, grid}};

    world ->obstacles[0] = (Object){{windowSize.x/4, windowSize.y/4}, {grid, windowSize.y/2}};
    world ->obstacles[1] = (Object){{3*windowSize.x/4, windowSize.y/4}, {grid, windowSize.y/2}};
    world ->obstacles[2] = (Object){{0, 0}, {0, 0}};
    world ->obstacles[3] = (Object){{1, 1}, {0, 0}};

    world ->food.spawn2 = &spawn2;
    world -> food.object.size = (vec2){grid, grid};
    world -> food.spawn2(world, world -> food_life_on_reset); // może crashowac
    world -> GameOver = 0;

    world ->head = malloc(sizeof(SnakePart));
    world ->head ->object = (Object){{windowSize.x/2, windowSize.y/2}, {grid, grid}};
    world ->head ->moveDir = (vec2){0, -1};
    world ->head ->New = 0;
    world ->head ->next = 0;

    return world;
}
//**********************************************************************************************************************
World* initWorld3(vec2 windowSize, int grid)
{
    World* world = malloc(sizeof(World));

    world -> windowSize = windowSize;
    world -> grid = grid;
    world -> food_life_on_reset = 60.0;

    world -> walls[0] = (Object){{0, 0}, {grid, windowSize.y}};
    world -> walls[1] = (Object){{windowSize.x - grid, 0}, {grid, windowSize.y}};
    world -> walls[2] = (Object){{0, 0}, {windowSize.x, grid}};
    world -> walls[3] = (Object){{0, windowSize.y - grid}, {windowSize.x, grid}};

    world ->obstacles[0] = (Object){{windowSize.x/4, windowSize.y/4}, {windowSize.x/2, grid}};
    world ->obstacles[2] = (Object){{3*windowSize.x/4, windowSize.y/4}, {grid, windowSize.y/2}};
    world ->obstacles[3] = (Object){{windowSize.x/4, windowSize.y/4}, {grid, windowSize.y/2}};
    world ->obstacles[1] = (Object){{2, 2}, {0, 0}};

    world ->food.spawn2 = &spawn2;
    world -> food.object.size = (vec2){grid, grid};
    world -> food.spawn2(world, world -> food_life_on_reset); // może crashowac
    world -> GameOver = 0;

    world ->head = malloc(sizeof(SnakePart));
    world ->head ->object = (Object){{windowSize.x/2, windowSize.y/2}, {grid, grid}};
    world ->head ->moveDir = (vec2){0, -1};
    world ->head ->New = 0;
    world ->head ->next = 0;

    return world;
}
//**********************************************************************************************************************
World* initWorld4(vec2 windowSize, int grid)
{
    World* world = malloc(sizeof(World));

    world -> windowSize = windowSize;
    world -> grid = grid;
    world -> food_life_on_reset = 60.0;

    world -> walls[0] = (Object){{0, 0}, {grid, windowSize.y}};
    world -> walls[1] = (Object){{windowSize.x - grid, 0}, {grid, windowSize.y}};
    world -> walls[2] = (Object){{0, 0}, {windowSize.x, grid}};
    world -> walls[3] = (Object){{0, windowSize.y - grid}, {windowSize.x, grid}};

    world ->obstacles[0] = (Object){{windowSize.x/4, windowSize.y/4}, {windowSize.x/8, windowSize.y/8}};
    world ->obstacles[1] = (Object){{windowSize.x/4, 5*windowSize.y/8}, {windowSize.x/8, windowSize.y/8}};
    world ->obstacles[2] = (Object){{5*windowSize.x/8, windowSize.y/4}, {windowSize.x/8, windowSize.y/8}};
    world ->obstacles[3] = (Object){{5*windowSize.x/8, 5*windowSize.y/8}, {windowSize.x/8, windowSize.y/8}};

    world ->food.spawn2 = &spawn2;
    world -> food.object.size = (vec2){grid, grid};
    world -> food.spawn2(world, world -> food_life_on_reset);
    world -> GameOver = 0;

    world ->head = malloc(sizeof(SnakePart));
    world ->head ->object = (Object){{windowSize.x/2, windowSize.y/2}, {grid, grid}};
    world ->head ->moveDir = (vec2){0, -1};
    world ->head ->New = 0;
    world ->head ->next = 0;

    return world;
}
