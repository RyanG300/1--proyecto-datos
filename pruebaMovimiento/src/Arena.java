import javax.swing.*;
import java.awt.*;

public class Arena extends JPanel {
    //Constructor
    public Arena(int filas, int columnas) {
        setLayout(new GridLayout(filas, columnas));
        // Añadir celdas
        for (int i = 0; i < filas * columnas; i++) {
            JPanel celda= new JPanel();
            celda.setBorder(BorderFactory.createLineBorder(Color.BLACK));
            celda.setBackground(Color.LIGHT_GRAY);
            add(celda);
        }
    }
}
