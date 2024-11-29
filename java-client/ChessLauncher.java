import javafx.application.Application;
import javafx.application.Platform;
import javafx.concurrent.Task;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;

public class ChessLauncher extends Application {
    private Socket socket;
    private PrintWriter out;

    @Override
    public void start(Stage primaryStage) {
        VBox vbox = new VBox(10);
        vbox.setStyle("-fx-padding: 20; -fx-alignment: center;");

        Label label = new Label("Enter the server IP address:");
        TextField ipField = new TextField();
        ipField.setPromptText("E.g., 127.0.0.1");

        Button connectButton = new Button("Connect");
        Label errorLabel = new Label();
        errorLabel.setStyle("-fx-text-fill: red;");

        connectButton.setOnAction(event -> {
            String serverAddress = ipField.getText().trim();
            int port = 8080; // Update with the correct port number

            if (validateIP(serverAddress)) {
                // Use a background Task for connecting to the server
                Task<Void> connectTask = new Task<>() {
                    @Override
                    protected Void call() {
                        try {
                            socket = new Socket(serverAddress, port); // Blocking operation
                            out = new PrintWriter(socket.getOutputStream(), true);

                            // Connection successful; update UI on JavaFX thread
                            Platform.runLater(() -> {
                                primaryStage.close();
                                try {
                                    ChessBoard chessBoard = new ChessBoard(socket, out); // Pass socket and writer
                                    chessBoard.start(new Stage());
                                } catch (Exception e) {
                                    showError("Error launching the chess game: " + e.getMessage());
                                }
                            });
                        } catch (IOException e) {
                            // Handle error on JavaFX thread
                            Platform.runLater(() -> showError("Error: Unable to connect to the server."));
                        }
                        return null;
                    }
                };

                // Run the task in a background thread
                new Thread(connectTask).start();
            } else {
                showError("Error: Invalid IP address.");
            }
        });

        vbox.getChildren().addAll(label, ipField, connectButton, errorLabel);

        Scene scene = new Scene(vbox, 300, 150);
        primaryStage.setTitle("Chess Game - Enter Server IP");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private boolean validateIP(String ip) {
        return true;
    }

    private void showError(String message) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Error");
        alert.setHeaderText(null);
        alert.setContentText(message);
        alert.showAndWait();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
