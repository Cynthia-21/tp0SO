package ar.edu.utn.frba.dds.fuenteestatica.models.domain.exceptions;

public class ErrorImportacionCSVException extends RuntimeException {
  public ErrorImportacionCSVException(Throwable cause) {
    super("Error al importar archivo CSV", cause);
  }
}
