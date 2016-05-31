/* Armemos en este archivo las ideas para la impementacion
** Para que no sea un quilombo, el primer nivel de
** indentacion indica de que "Idea" es, el segundo
** que archivo y el siguiente que funcion.
** Es para tener una idea de la imprementacion,
** parecido a un .h
*/

IDEA1
  blobsfront.c
    main(){
      //CORE
      //Faltan el menu y todo eso
      if ((command = pedirMov(player)) != null) {
        blobCount[player] += move(player, command); //devuelve 1 si divide
        conquer(player, command, blobCount);
        render(tablero, dimx, dimy);
      }
      else {
        // Termino el juego, sumar espacios al ganador
        // y mostrar shit
        // Vuelve al menu???
      }
    }

    void render(char** tablero, int dimx, int dimy) {
      loop(todoeltablero) {
        modo caca:
          imprime las A y las Z bien villa

        modo l33t m4dn3$$:
          le agregamos color //no es complicated
      }
    }

  blobsback.c
    movimiento pedirMov(int player){
      command = null; //WIP
      if(hayMovValido) {
        do {
          command = Pedir comando (creo que scanf no sirve)
        }while(!validarMov(command));
      }
      return command;
    }

    int validarMov(movimiento command, int player) {
      int esValido = 1;
      if(command.inicio no es de player) esValido = 0;
      else if(command.fin no esta vacio) esValido = 0;
      else if(no esta dentro del tablero) esValido = 0;
      else if(distancia en x > 2 || idem y) esValido = 0;

      return esValido;
    }

    int move(int player, movimiento command){
      int seDivide = 1;
      tablero[cmd.fin.x][cmd.fin.y] = player; //Hay que ponerle A o Z
      if(distancia x = 2 || disty = 2) {
        tablero[cmd.ini.x][cmd.ini.y] = '0'; //Salta
        seDivide = 0;
      }
      return seDivide;
    }

    void conquer(int player, movimiento command, int blobcount[]) {
      loop(alrededorde command.fin) { //NO SALIR DEL TABLERO (NULL PTR)
        if(tablero[i][j] == alOtroPlayer){
          tablero[i][j] = player;
          blobcount[player]++;
          blobcount[otro]--;
        }
      }
    }
