package domain.exceptions;

public class ErrorImportacionCSVException extends RuntimeException {
  public ErrorImportacionCSVException(Throwable cause) {
    super("Error al importar archivo CSV", cause);
  }
}
