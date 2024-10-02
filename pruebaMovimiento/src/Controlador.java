import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

public class Controlador extends KeyAdapter {
    private Personaje personaje; //Personaje a controlar
    private int filas;
    private int columnas;

    public Controlador(Personaje personaje, int filas, int columnas) {
        this.personaje = personaje;
        this.filas = filas;
        this.columnas = columnas;

    }

    @Override
    public void keyPressed(KeyEvent e) {
        int key = e.getKeyCode(); // Obtiene la tecla
        int nuevaFila = personaje.getFila(); // Fila actual
        int nuevaColumna = personaje.getColumna(); // Columna actual

        if (key == KeyEvent.VK_UP && nuevaFila > 0) {
            nuevaFila--; // mueve hacia arriba
        } else if (key == KeyEvent.VK_DOWN && nuevaFila < filas - 1) {
            nuevaFila++; // mueve hacia abajo
        } else if (key == KeyEvent.VK_LEFT && nuevaColumna > 0) {
            nuevaColumna--; // mueve hacia la izquiera
        } else if (key == KeyEvent.VK_RIGHT && nuevaColumna < columnas - 1) {
            nuevaColumna++; // mueve hacia la derecha
        }

        // Llama a Funcion que mueve al personaje
        personaje.moverPersonaje(nuevaFila, nuevaColumna);
    }
}
