# EDAversi

## Integrantes del grupo y contribución al trabajo de cada integrante

Grigera Candelaria

Schiaffino Ariana

Decidimos trabajar juntas a lo largo del desarrollo del trabajo.

## Parte 1: Generación de movimientos válidos y algoritmo de jugada

Primero, determinamos como ibamos a generar las direcciones validas para validar un movimiento 
y asi agregarlo al vector validMoves. Recorrimos el tablero y , por cada lugar, si estaba vacio,
se buscaba por lo menos una direccion que cumpliera con el requito de capturar al menos una pieza 
del oponente y, para esto, que al final hubiera una pieza del mismo jugador. Pensamos en generar
las ocho direcciones de la siguiente forma:
![image](https://github.com/user-attachments/assets/1d0af242-1b7c-40e4-92f9-14917d12a900)

Asi, por cada direccion, se entra a un bucle while que se mueve en ese sentido mientras que el 
movimiento sea valido y sea una ficha del oponente. Cuando sale del for, podria ser por cualquiera 
de las dos opciones, por esto implementamos la flag foundOpponent la cual, si esta activada, permite
entrar a una condicion que chequea que la siguiente pieza sea propia para poder finalmente efectuar 
el movimiento. Si es propia, significa que ya encontro una direccion valida para moverse: ya es 
suficiente para incluirla como movimiento valido y luego pasar a la siguiente pieza (ponemos los 
iteradores del for con un valor particular para que no vuelvan a entrar (nos parecio la mejor manera de 
realizarlo para asi evitar usar multiples breaks y hacer el codigo mas legible).
Para validar nuestro generador de movimientos validos, probamos intentar colocar fichas en lugares 
que sabiamos que no se podian dadas las reglas del juego. Luego, en la funcion playMove, se genera 
el vector de movimientos validos y se verifica que el movimiento que se quiere hacer este efectivamente 
en dicho vector, si no lo esta: retorna falso ya que logicamente no se puede llevar a cabo.
Si el movimiento se puede llevar a cabo, creamos una variable auxiliar para moverla en la direccion 
elegida mientras que esta sea valida y recorra fichas del oponente. Una vez que sale del bucle, ocurre
lo mismo que en el caso anterior: puede ser por dos motivos y por esto usamos la misma flag foundOpponent. 
Ahora bien, si la pieza que sigue es una propia, significa que debemos pintar del color propio todo lo que
hemos recorrido en el bucle anterior, es por eso que ahora hay otro bucle que retrocede los pasos anteriores
y, por cada retroceso, pinta la casilla del color propio.

Ademas, para chequear el correcto funcionamiento de nuestro codigo implementamos diversas funciones.
Primero, inicializamos el tablero con 4 fichas, conociendo las posiciones, de manera tal que se vea asi:
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . W B . . .
. . . B W . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
Luego, simulamos ser el jugador blanco y llamamos a getValidMoves. Luego, verificamos que efectivamente el vector contenia 4 
elementos: los validos posibles para jugar.
Luego, llamamos a la funcion playMove y verificamos que efectivamente el tablero se haya modificado correctamente.
Tambien probamos en intentar efectuar un movimiento invalido y verificamos que efectivamente no se haya modificado el tablero.
Finalmente, pintamos todas las fichas menos una de color blanco, llamamos a getValidMoves siendo el jugador negro y verificamos 
que, en efecto, el vector devuelto estaba vacio.
Para llevar a cabo todo lo anterior, hicimos uso del siguiente codigo:

void initialBoard(GameModel &model)
{
    initBoard(model);
    setBoardPiece(model, {3, 3}, PIECE_WHITE);
    setBoardPiece(model, {3, 4}, PIECE_BLACK);
    setBoardPiece(model, {4, 3}, PIECE_BLACK);
    setBoardPiece(model, {4, 4}, PIECE_WHITE);
}

// Test 1
void testInitialValidMoves() 
{
    GameModel model;
    setupInitialBoard(model);
    model.currentPlayer = PLAYER_WHITE;
    Moves moves;
    getValidMoves(model, moves);
    assert(moves.size() == 4);
}

//test 2
void testNoValidMoves() {
    GameModel model;
    initBoard(model);
    for (int j = 1; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            setBoardPiece(model, {i, j}, PIECE_WHITE);
        }
    }
    model.currentPlayer = PLAYER_BLACK;
    Moves moves;
    getValidMoves(model, moves);
    assert(moves.empty());
}

## Parte 2: Implementación del motor de IA

En la funcion getBestMove, conseguimos los movimientos validos y creamos  una instancia de Square para guardar
efectivamente el movimiento mas conveniente. Iteramos sobre cada movimiento valido (que seria la primera generacion 
de hijos del nodo raiz) y, por cada una de ellas, creamos un modelo auxiliar y jugamos alli el movimiento (ya que no 
podemos hacerlo sobre el tablero original y lo necesitamos para, a partir de ellos, generar los nuevos movimientos
validos y, por lo tanto, sus nodos hijos para asi ir desarrollando el arbol, donde cada nivel representa una jugada 
y la de cada nodo los posibles movimientos (observemos que la cantidad de nodos por nivel es equivalente a la cantidad 
de elementos del vector de movimientos validos que generemos).
Ahora bien, por cada nodo de la primera generacion se llama a la funcion minimax que generara los nodos hijos.
Observamos que tiene parametros que aun no hemos aclarado ya que se refiere a la poda del arbol que trabajaremos en
la parte 3, sin embargo, esta funcion se llamara recursivamente hasta que se cumpla alguna de las dos condiciones de 
salida : se hayan recorrido una cantidad maxima de nodos preestablecidos o se haya llegado a una determinada profundidad 
(ya que antes intentamos no "podar" el arbol y a la inteligencia artificial le demoraba mucho elegir su proximo movimiento,
lo que hacia que el programa se colgara). Cuando se sale del primer nodo de la primera generacion (si es que sale, podrian 
cumplirse alguna de las condiciones antes de explorar el segundo), se repite el mismo proceso con el segundo nodo de la
primera generacion y asi sucesivamente hasta que las condiciones lo permitan: asi es como logramos generar las ramas de 
nuestro arbol.
Ahora bien, la funcion minimax al principio chequea ciertas condiciones como las dos de finalizacion que mencionamos 
anteriormente y ademas si el juego ha terminado o si el vector de movimientos validos esta vacio. Si se cumple alguna, 
llama a una funcion checkBoard que determinara el "valor" de ese nodo basandose en la cantidad de fichas y en las esquinas.
Dependiendo de cual jugador es el movimiento, suma uno por cada ficha propia y resta uno por cada una del oponente, dependiendo de
cuantas esquinas, suma cien por cada una propia y resta cien por cada una del oponente: de esta forma conseguimos una forma de evaluar 
que tan buena o mala es una jugada y, asi, asignarle el valor al nodo.
Ademas tiene un argumento whoIsPlaying que, dependiendo con cual es llamado, ejecuta una cierta logica y luego se vuelve 
a llamar pero con el jugador opuesto. La logica es la siguiente: si soy MAX, es decir el argumento es verdadero, voy a 
quedarme con el valor que maximice mi ganancia (el mas alto de los valores de los nodos hijos), hacemos uso de la funcion 
max para determinar el valor maximo del nodo que recorrio antes con el que recorre ahora (bestValue y value respectivamente). Antes de salir, verifica que no se haya cumplido la condicion de fin y devuelve ese valor maximo encontrado.Por otro lado, si soy MIN ejecuto la logica contraria: me quedo con el minimo valor para minimizar la ganancia del oponente.
El resumen de la logica:
![image](https://github.com/user-attachments/assets/3d45b81c-f53f-4bf1-a5d3-d6312cc0dcd2)

## Parte 3: Poda del árbol

Como se menciono anteriormente, intentamos correr el programa sin poner condiciones y no funciono. Es por esto que creamos
dos constantes que representan el valor maximo de nodos visitados y el nivel maximo al que se puede llegar. Optamos por usar
ambas condiciones ya que, como recorremos el arbol en profundidad, si poniamos solo un maximo de nodos tal vez solo se 
desarrollaba una sola rama del arbol (osea un solo nodo de la primera generacion) y eso tampoco nos convenia ya que quitaba 
muchas posibilidades. Al combinar ambas, podemos lograr que se recorra mas de un nodo de la primera generacion.

## Documentación adicional


## Bonus points
Implementamos la poda alpha-beta dentro del algoritmo minimax. Cuando jugaba MAX y se determinaba el best value (antes de 
implementar esta poda, bestvalue se retornaba y representaba el valor maximo entre los hijos), ahora bien alpha representa
el valor maximo que puede tomar hasta ahora MAX y beta representa el valor minimo que puede tomar hasta ahora MIN. En nuestro
caso, utilizamos esta poda para evitar recorrer ramas que, dada la logica de minimax, sabiamos o podiamos predecir que no
iban a ser seleccionadas. Pudimos determinar que cuando se cumplia la condicion de que beta era menor que alpha, ya no tenia
caso seguir explorando la rama, por que? si soy MAX y encuentro un valor que es mayor a beta, podo ya que se que MIN no 
eligira el camino (ya que busca el menor valor y, si se topa con uno mayor al valor minimo que encontro hasta ahora, es
evidente y logico que no le conviene y por lo tanto no lo elija), entonces aqui ya me ahorro de recorrer muchas ramas que,
por mas que presenten buenas jugadas, sera imposible que sean elegidas por el otro jugador. Ahora bien, si soy MIN y encuentro
un valor menor a alpha, podo ya que tengo la certeza de que MAX nunca lo eligiria: por la misma razon que antes ya que busca
maximizar la ganancia y si encuentra un valor mas chico del maximo que encontro hasta ahora, tampoco tiene sentido que opte
por el ya que no le convendria.
Concluimos entonces que esta poda permite explorar el arbol en mayor profundidad y en mayor amplitud, permitiendo asi mas 
posibilidades de una buena eleccion de jugada por parte de la IA.
