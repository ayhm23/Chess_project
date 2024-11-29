import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.GridPane;

import java.util.Optional;
import java.util.function.Consumer;

public class PieceManager {
    private final GridPane grid;
    private final int tileSize;
    private final String[][] boardState = new String[8][8]; // Tracks the board state
    private ImageView selectedPiece = null; // Currently selected piece
    private int initialRow; // Initial row of the selected piece
    private int initialCol; // Initial column of the selected piece
    private final Consumer<String> moveSender; // Functional interface for sending moves

    public PieceManager(GridPane grid, int tileSize, Consumer<String> moveSender) {
        this.grid = grid;
        this.tileSize = tileSize;
        this.moveSender = moveSender;
    }

    // Initialize all chess pieces
    public void initializePieces() {
        // Add black pieces
        addPiece("rook_black.png", 0, 0);
        addPiece("knight_black.png", 0, 1);
        addPiece("bishop_black.png", 0, 2);
        addPiece("queen_black.png", 0, 3);
        addPiece("king_black.png", 0, 4);
        addPiece("bishop_black.png", 0, 5);
        addPiece("knight_black.png", 0, 6);
        addPiece("rook_black.png", 0, 7);
        for (int col = 0; col < 8; col++)
            addPiece("pawn_black.png", 1, col);

        // Add white pieces
        addPiece("rook_white.png", 7, 0);
        addPiece("knight_white.png", 7, 1);
        addPiece("bishop_white.png", 7, 2);
        addPiece("queen_white.png", 7, 3);
        addPiece("king_white.png", 7, 4);
        addPiece("bishop_white.png", 7, 5);
        addPiece("knight_white.png", 7, 6);
        addPiece("rook_white.png", 7, 7);
        for (int col = 0; col < 8; col++)
            addPiece("pawn_white.png", 6, col);
    }

    private boolean isMyTurn = false; // Tracks whether it's the player's turn

    public void setMyTurn(boolean myTurn) {
        isMyTurn = myTurn;
    }

    // Updated addPiece method to disable interaction for black pieces
    private void addPiece(String imageName, int row, int col) {
        Image image = new Image(getClass().getResourceAsStream("/resources/images/" + imageName));
        ImageView piece = new ImageView(image);
        piece.setFitWidth(tileSize * 0.8);
        piece.setFitHeight(tileSize * 0.8);

        piece.setUserData(imageName); // Store metadata (piece type/color)

        // Only allow interaction for white pieces
        if (imageName.contains("white")) {
            piece.setOnMousePressed(event -> {
                if (isMyTurn) {
                    onPiecePressed(event, piece);
                }
            });
            piece.setOnMouseDragged(event -> {
                if (isMyTurn) {
                    onPieceDragged(event, piece);
                }
            });
            piece.setOnMouseReleased(event -> {
                if (isMyTurn) {
                    onPieceReleased(event, piece);
                }
            });
        }

        GridPane.setRowIndex(piece, row);
        GridPane.setColumnIndex(piece, col);
        grid.getChildren().add(piece);
        boardState[row][col] = imageName; // Update board state
    }

    // Handle mouse press event
    private void onPiecePressed(MouseEvent event, ImageView piece) {
        selectedPiece = piece;
        initialRow = Optional.ofNullable(GridPane.getRowIndex(piece)).orElse(-1);
        initialCol = Optional.ofNullable(GridPane.getColumnIndex(piece)).orElse(-1);
    }

    // Handle piece dragging
    private void onPieceDragged(MouseEvent event, ImageView piece) {
        piece.setTranslateX(event.getSceneX() - piece.getLayoutX() - tileSize / 2);
        piece.setTranslateY(event.getSceneY() - piece.getLayoutY() - tileSize / 2);
    }

    // Handle mouse release event
    // Handle mouse release event
    private void onPieceReleased(MouseEvent event, ImageView piece) {
        int newCol = (int) ((event.getSceneX() - grid.getLayoutX()) / tileSize);
        int newRow = (int) ((event.getSceneY() - grid.getLayoutY()) / tileSize);

        // Check if the piece is released at its original position
        if (newRow == initialRow && newCol == initialCol) {
            snapBack(piece);
            return;
        }

        // Validate the move
        if (isValidMove(newRow, newCol)) {
            String pieceType = ((String) piece.getUserData()).split("_")[0];
            pieceType = pieceType.substring(0, 1).toUpperCase() + pieceType.substring(1).toLowerCase();

            String move = pieceType + " " + initialCol + "" + (7 - initialRow) + " " + newCol + "" + (7 - newRow);
            moveSender.accept(move); // Send move to server for validation

            if (boardState[newRow][newCol] != null) {
                handlePieceCapture(newRow, newCol, piece); // Remove the opponent's piece
            } else {
                // Move your piece to the new position
                movePiece(piece, newRow, newCol);
            }
        } else {
            // Snap back if move is invalid
            snapBack(piece);
        }
    }

    // Validate if the move is within bounds and valid
    private boolean isValidMove(int row, int col) {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    // Handle capturing a piece on the destination square
    // Handle capturing a piece on the destination square
    private void handlePieceCapture(int newRow, int newCol, ImageView capturingPiece) {
        // Remove the piece being captured (from GridPane and boardState)
        grid.getChildren().removeIf(node -> {
            if (node instanceof ImageView &&
                    GridPane.getRowIndex(node) == newRow &&
                    GridPane.getColumnIndex(node) == newCol) {
                boardState[newRow][newCol] = null; // Clear the captured piece's state
                return true; // Remove the captured piece from the GridPane
            }
            return false;
        });

        // Move the capturing piece to the captured piece's position
        GridPane.setRowIndex(capturingPiece, newRow);
        GridPane.setColumnIndex(capturingPiece, newCol);

        // Update the board state
        boardState[initialRow][initialCol] = null; // Clear the original position of the capturing piece
        boardState[newRow][newCol] = (String) capturingPiece.getUserData(); // Place the capturing piece in the new
                                                                            // position

        // Reset translation offsets for the capturing piece
        capturingPiece.setTranslateX(0);
        capturingPiece.setTranslateY(0);

        // Debug information (optional)
        System.out.println("Captured piece replaced at: (" + newRow + ", " + newCol + ")");
        // printBoardState(); // Optional: Log the updated board state
    }

    // Add this method in PieceManager
    public void snapBackSelectedPiece() {
        if (selectedPiece != null) {
            snapBack(selectedPiece);
        }
    }

    // Move the selected piece to the new position
    private void movePiece(ImageView piece, int newRow, int newCol) {
        // Update the visual position on the GridPane
        GridPane.setRowIndex(piece, newRow);
        GridPane.setColumnIndex(piece, newCol);

        // Update the logical board state
        boardState[initialRow][initialCol] = null; // Clear the original position
        boardState[newRow][newCol] = (String) piece.getUserData(); // Set the new position

        // Reset translation offsets
        piece.setTranslateX(0);
        piece.setTranslateY(0);
    }

    // Snap the piece back to its original position
    private void snapBack(ImageView piece) {
        if (piece != null) {
            piece.setTranslateX(0);
            piece.setTranslateY(0);
            GridPane.setRowIndex(piece, initialRow);
            GridPane.setColumnIndex(piece, initialCol);
        }
    }

    public void updateBoardFromOpponentMove(String move) {
        String[] parts = move.split(" ");
        if (parts.length == 3) {
            String pieceType = parts[0];
            int fromCol = Character.getNumericValue(parts[1].charAt(0));
            int fromRow = Character.getNumericValue(parts[1].charAt(1));
            int toCol = Character.getNumericValue(parts[2].charAt(0));
            int toRow = Character.getNumericValue(parts[2].charAt(1));

            grid.getChildren().removeIf(node -> node instanceof ImageView &&
                    GridPane.getRowIndex(node) == toRow &&
                    GridPane.getColumnIndex(node) == toCol // Capture logic
            );

            updateBoard(pieceType, fromCol, fromRow, toCol, toRow);
        }
    }

    // Update board based on a server move
    public void updateBoard(String pieceType, int fromCol, int fromRow, int toCol, int toRow) {
        grid.getChildren()
                .removeIf(node -> GridPane.getRowIndex(node) == toRow && GridPane.getColumnIndex(node) == toCol);

        for (var node : grid.getChildren()) {
            if (GridPane.getRowIndex(node) == fromRow && GridPane.getColumnIndex(node) == fromCol
                    && node instanceof ImageView) {
                ImageView piece = (ImageView) node;
                GridPane.setRowIndex(piece, toRow);
                GridPane.setColumnIndex(piece, toCol);

                // Update board state
                boardState[fromRow][fromCol] = null;
                boardState[toRow][toCol] = (String) piece.getUserData();
                break;
            }
        }
    }
}
