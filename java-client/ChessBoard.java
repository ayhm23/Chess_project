import javafx.application.Application;
import javafx.application.Platform;
import javafx.scene.Scene;
import javafx.scene.layout.GridPane;
import javafx.stage.Stage;
import javafx.scene.control.Alert;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;



import javafx.scene.control.Label;
import javafx.scene.layout.VBox;

public class ChessBoard extends Application {
    private Socket socket;
    private BufferedReader in;
    private PrintWriter out;
    private final int tileSize = 80; // Size of each tile
    private final int boardSize = 8; // 8x8 board
    private PieceManager pieceManager;
    private Label gameStatusLabel; // Label to display game status

    public ChessBoard() {
        // Empty constructor required for JavaFX Application class
    }
    public ChessBoard(Socket socket, PrintWriter out) {
        this.socket = socket;
        this.out = out;
    }
    public ChessBoard(Socket socket) {
        this.socket = socket;

        try {
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            out = new PrintWriter(socket.getOutputStream(), true);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    
    @Override
public void start(Stage primaryStage) {
    // Initialize socket connection (if not passed in the constructor)
    try {
        if (socket == null) {
            socket = new Socket("localhost", 8080); // Connect to server
        }
        if (in == null || out == null) {
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            out = new PrintWriter(socket.getOutputStream(), true);
        }
    } catch (IOException e) {
        showAlert("Connection Error", "Could not connect to the server.");
        return; // Exit if the connection fails
    }

    // Initialize UI components
    GridPane grid = new GridPane();
    pieceManager = new PieceManager(grid, tileSize, this::sendMove);

    // Initialize chessboard tiles
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            ChessTile tile = new ChessTile((row + col) % 2 == 0, tileSize, row, col);
            grid.add(tile, col, row);
        }
    }

    // Initialize chess pieces
    pieceManager.initializePieces();

    // Create a label for game status
    gameStatusLabel = new Label("Waiting for game to start...");
    gameStatusLabel.setStyle("-fx-font-size: 16px; -fx-font-weight: bold;");

    VBox root = new VBox(10, gameStatusLabel, grid);
    root.setStyle("-fx-padding: 20; -fx-alignment: center;");

    // Create the scene
    Scene scene = new Scene(root, tileSize * boardSize, tileSize * boardSize + 50);
    primaryStage.setTitle("Chess Game - Online");
    primaryStage.setScene(scene);
    primaryStage.show();

    // Start a thread to listen for server messages
    if (in != null) {
        new Thread(this::listenToServer).start();
    } else {
        showAlert("Error", "Server communication is not set up properly.");
    }
}

    private void sendMove(String move) {
        if (out != null) {
            out.println(move); // Send the move to the server
        } else {
            showAlert("Communication Error", "Could not send the move to the server.");
        }
    }

    private void listenToServer() {
        if (in == null) {
            Platform.runLater(() -> showAlert("Error", "Cannot read from server. Connection not established."));
            return;
        }
    
        try {
            String message;
            while ((message = in.readLine()) != null) {
                String finalMessage = message; // Capture message for JavaFX thread
                Platform.runLater(() -> processServerMessage(finalMessage));
            }
        } catch (IOException e) {
            Platform.runLater(() -> showAlert("Connection Error", "Lost connection to the server."));
        }
    }
    
    

    private void processServerMessage(String message) {
        if (message.startsWith("Your move")) {
            gameStatusLabel.setText("Your move. Make your move.");
            pieceManager.setMyTurn(true); // Enable interaction for your pieces
        } else if (message.startsWith("Waiting for opponent")) {
            gameStatusLabel.setText("Waiting for opponent's move...");
            pieceManager.setMyTurn(false); // Disable interaction during opponent's turn
        } else if (message.startsWith("MOVE_VALID")) {
            System.out.println("Move accepted by the server.");
            gameStatusLabel.setText("Move accepted. Waiting for opponent...");
        } else if (message.startsWith("-1")) { // Invalid move
            gameStatusLabel.setText("Invalid move. Try again.");
            Platform.runLater(() -> pieceManager.snapBackSelectedPiece());
        } else if (message.startsWith("Opponent move")) {
            String opponentMove = message.split(":")[1].trim();
            System.out.println("Opponent moved: " + opponentMove);
            gameStatusLabel.setText("Opponent moved. Your turn.");
            pieceManager.updateBoardFromOpponentMove(opponentMove);
            pieceManager.setMyTurn(true); // Enable interaction after opponent's move
        } else if (message.startsWith("Checkmate")) {
            showAlert("Game Over", message);
            gameStatusLabel.setText("Game Over: " + message);
        } else {
            System.out.println("Server response: " + message);
        }
    }
    
    
    
    

    private void showAlert(String title, String content) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle(title);
        alert.setHeaderText(null);
        alert.setContentText(content);
        alert.showAndWait();
    }

    public static void main(String[] args) {
        launch(args); // Start the JavaFX application
    }
}
