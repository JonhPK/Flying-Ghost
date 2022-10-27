#include "raylib.h"
#include <stdio.h>

#define MAX_OBSTACULOS 100
#define OBSTACULOS_WIDTH 80
#define FANTASMA_RAIO 105


typedef struct Obstaculos {
    Rectangle rec;
    Color cor;
    bool active;
} Obstaculos;

typedef struct Fantasma {
    int raio;
    Vector2 pos;
}Fantasma;


int main()  {
    
    
    int screenWidth = 800, screenHeight = 450;
    

    InitWindow(screenWidth, screenHeight, "Flying Ghost");
    InitAudioDevice();
    
    SetTargetFPS(60);
    
   
    //Importando Imagens/Texturas para o programa
    Texture2D Fantasma2 = LoadTexture("C:/Users/zedya/Downloads/Projeto/Flying_Ghost/fantasma2.png");
    Texture2D Fundo = LoadTexture("C:/Users/zedya/Downloads/Projeto/Flying_Ghost/Fundo2.png");
    
    
    //Importando Músicas do jogo
    Music Musica = LoadMusicStream("C:/Users/zedya/Downloads/Projeto/Flying_Ghost/Musica_Jogo.mp3");
    PlayMusicStream(Musica);
    
    
    static Obstaculos obstaculo[MAX_OBSTACULOS * 2] = { 0 };
    static Vector2 obstaculoPos[MAX_OBSTACULOS] = { 0 };
    static float obstaculoSpeedX = 0;
    static bool superfx = false;

    
    FILE *file;

    static Fantasma fantasma = {0};
    bool gameover = false;
    int Hiscore = 0;
    int score = 0;
    bool pausa = false;
    bool menu = true;
    Vector2 icone = {-100.0f, -100.0f};

    gameover = false;
    superfx = false;
    pausa = false;
    

    file = fopen("Hiscore.txt", "r"); //Abre o Arquivo de Hiscore e escreve na tela
    fscanf(file, "%d", &Hiscore); 
    fclose(file);

    
    void iniciarJogo(){ //Inicia as variaveis do jogo
    
    
    
        
        fantasma.raio = FANTASMA_RAIO;
        fantasma.pos = (Vector2){80, screenWidth/2 - fantasma.raio};
        obstaculoSpeedX = 3;

        for (int i = 0; i < MAX_OBSTACULOS; i++)    //Gera as variações das posições dos obstaculos
        {
            obstaculoPos[i].x = 400 + 280*i;
            obstaculoPos[i].y = -GetRandomValue(0, 120);
        }

        for (int i = 0; i < MAX_OBSTACULOS*2; i += 2) //Gera a posição e os tamanhos dos obstaculo
        {
            obstaculo[i].rec.x = obstaculoPos[i/2].x;
            obstaculo[i].rec.y = obstaculoPos[i/2].y;
            obstaculo[i].rec.width = OBSTACULOS_WIDTH;
            obstaculo[i].rec.height = 210;

            obstaculo[i+1].rec.x = obstaculoPos[i/2].x;
            obstaculo[i+1].rec.y = 600 + obstaculoPos[i/2].y - 255;
            obstaculo[i+1].rec.width = OBSTACULOS_WIDTH;
            obstaculo[i+1].rec.height = 255;

            obstaculo[i/2].active = true;   
        }
        
        score = 0;
        gameover = false;
        superfx = false;
        pausa = false;
    }
    
    
    iniciarJogo();
    gameover = false;
 
    
    while(!WindowShouldClose()) {
        
        
        UpdateMusicStream(Musica);
        
        
        if (IsKeyPressed('P')){ //Pausa o jogo
            pausa = !pausa;
        }
       
        BeginDrawing(); //Começar a desenhar       
        ClearBackground(BLANK); //Escolhe uma cor de fundo
        
            if(!menu){
            
                if (!gameover) {
                    
                    if (!pausa) {
                    
                        if(IsKeyPressed(KEY_ENTER)){
                            return NULL;
                        }
                    
                        for (int i = 0; i < MAX_OBSTACULOS; i++){ //Faz o movimento dos obstaculos
                            obstaculoPos[i].x -= obstaculoSpeedX;
                        }
                        
                        for (int i = 0; i < MAX_OBSTACULOS*2; i += 2)
                         {
                            obstaculo[i].rec.x = obstaculoPos[i/2].x;
                            obstaculo[i+1].rec.x = obstaculoPos[i/2].x;
                         }    

                        for (int i = 0; i < MAX_OBSTACULOS*2; i++) //Chegando as colisões
                            {
                                
                                if (CheckCollisionCircleRec(fantasma.pos , fantasma.raio, obstaculo[i].rec))
                                {
                                    gameover = true;
                                    pausa = false;
                                }
                                else if ((obstaculoPos[i/2].x < fantasma.pos.x) && obstaculo[i/2].active && !gameover)
                                {
                                    score += 100;
                                    obstaculo[i/2].active = false;

                                    superfx = true;

                                    if (score > Hiscore)
                                        Hiscore = score;
                                }
                            }
                       
                        if (IsKeyDown(KEY_SPACE)){ //Pressionando a tecla de Espaço
                            fantasma.pos.y -= 4;
                            
                        }else fantasma.pos.y += 2;
                        
                    }  
  
                
                DrawTexture(Fundo, 0, 0, RAYWHITE); //Desenhando a imagem de Fundo
                
                
                //Desenhando Obstaculos
                for (int i = 0; i < MAX_OBSTACULOS; i++)
                    {
                        DrawRectangleGradientV(    //Retangulo de cima
                        obstaculo[i*2].rec.x,
                        obstaculo[i*2].rec.y, 
                        obstaculo[i*2].rec.width,
                        obstaculo[i*2].rec.height, BLACK, ORANGE);
                        
                        DrawRectangleGradientV(    //Retangulo de baixo
                        obstaculo[i*2 + 1].rec.x, 
                        obstaculo[i*2 + 1].rec.y, 
                        obstaculo[i*2 + 1].rec.width, 
                        obstaculo[i*2 + 1].rec.height, RED, ORANGE); 
                    }
                    
                // Desenha o flashing fx (apenas um frame)
                if (superfx){
                    DrawRectangleGradientV(0, 0, screenWidth, screenHeight, ORANGE, RED);
                        superfx = false;
                    }
                
                DrawTexture(                     //Desenhando o Fantasma
                        Fantasma2,
                        fantasma.pos.x,
                        fantasma.pos.y - 150,
                        RAYWHITE);
                
                if (pausa){                         //Imprime na tela quando pausar o jogo
                    DrawText("Jogo Pausado", GetScreenWidth()/2 - MeasureText("Jogo Pausado", 30)/2, GetScreenHeight()/2 - 30,
                    30, RAYWHITE);
                }  

                DrawFPS(0, 0); //Imprime na tela o contador de FPS
                DrawText(TextFormat("%05i", score), 20, 20, 40, GRAY);
                DrawText(TextFormat("HI-SCORE: %05i", Hiscore), 20, 70, 20, LIGHTGRAY);

                } 
                else DrawText("PRESSSIONE 'ENTER' PARA JOGAR NOVAMENTE", GetScreenWidth()/2 - MeasureText("PRESSIONE 'ENTER' PARA JOGAR NOVAMENTE", 20)/2, GetScreenHeight()/2 - 50, 20, RAYWHITE);
                
                if(IsKeyPressed(KEY_ENTER)){
                    gameover = false;
                    iniciarJogo();
                }
            }
            else{ DrawTexture( // Cria o menu inical do jogo
                        Fantasma2,
                        320,
                        220, RAYWHITE);
                        
                DrawText("Iniciar", 330, 160, 40, RAYWHITE);
               
                menu = true;
                
               
                if(menu){ 
                    icone = GetMousePosition();
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    gameover = false;
                    iniciarJogo();
                    menu = false;
                    } 
                } 
            }
            EndDrawing(); //Encerra o desenho e exibe na tela
                
        }
 
        file = fopen("Hiscore.txt", "w"); //Cria um arquivo tipo .txt e escreve o Hiscore dentro
        fprintf(file, "%d", Hiscore);
        fclose(file);
        
    
    UnloadMusicStream(Musica);
    UnloadTexture(Fantasma2);
 
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
} 