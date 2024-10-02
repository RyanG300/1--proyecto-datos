import javax.swing.*;
import java.awt.*;

public class Main extends JFrame {

    // Constructor
    public Main(){
        setTitle("League Tec");
        setSize(800,600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        int filas = 10;
        int columnas = 10;

        // Crea la arena
        Arena arena = new Arena(filas, columnas);
        add(arena, BorderLayout.CENTER);

        // Inicializa el personaje en una posición de la arena
        Personaje personaje = new Personaje(0, 0, arena, columnas);
        JPanel celdaInicial = (JPanel) arena.getComponent(0);
        celdaInicial.add(personaje);

        // Inicializa el controlador de movimiento
        Controlador controlador = new Controlador(personaje, filas, columnas);
        arena.addKeyListener(new Controlador(personaje, filas, columnas));
        arena.setFocusable(true);

        setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new Main());
    }
}