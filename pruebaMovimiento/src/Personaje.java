import java.awt.*;
import javax.swing.*;

public class Personaje extends JLabel {
    private int fila; // Fila actual
    private int columna; // Columna actual
    private JPanel arena;
    private int columnas; // Total columnas

    public Personaje(int filaInicial, int columnaInicial, JPanel arena, int columnas){
        this.fila = filaInicial;
        this.columna = columnaInicial;
        this.arena = arena;
        this.columnas = columnas;
        setPreferredSize(new Dimension(50, 50));
    }

    //Genera un cuadrado, que debería ser el personaje **ignorar
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        g.setColor(Color.RED);
        g.fillRect(0, 0, getWidth(), getHeight());
    }

    public int getFila() {
        return fila;
    }

    public int getColumna(){
        return columna;
    }

    public void setFila(int fila){
        this.fila = fila;
    }

    public void setColumna(int columna){
        this.columna = columna;
    }



    public void moverPersonaje(int nuevaFila, int nuevaColumna){
        // Verifica si la nueva posición es†á dentro los límites
        if (nuevaFila < 0 || nuevaFila >= arena.getHeight() / 50 || nuevaColumna < 0 || nuevaColumna >= columnas);

        // Elimina el personaje de la celda actual
        JPanel celdaActual = (JPanel) arena.getComponent(fila * columnas + columna);
        celdaActual.remove(this);
        celdaActual.revalidate();
        celdaActual.repaint();

        // Actualiza la posición
        this.fila = nuevaFila;
        this.columna = nuevaColumna;

        //Agrega el personaje en la nueva celda
        JPanel nuevaCelda = (JPanel) arena.getComponent(nuevaFila * columnas + nuevaColumna);
        nuevaCelda.add(this);
        nuevaCelda.revalidate();
        nuevaCelda.repaint();
    }

}
