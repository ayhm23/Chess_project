import javafx.scene.layout.Region;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;

public class ChessTile extends Region {
    private final Rectangle square;
    private final boolean isLight;
    private final int row;
    private final int col;

    public ChessTile(boolean isLight, int size, int row, int col) {
        this.isLight = isLight;
        this.row = row;
        this.col = col;

        square = new Rectangle(size, size);
        square.setFill(isLight ? Color.BEIGE : Color.DARKGRAY);
        getChildren().add(square);
    }

    public int getRow() {
        return row;
    }

    public int getCol() {
        return col;
    }
}
