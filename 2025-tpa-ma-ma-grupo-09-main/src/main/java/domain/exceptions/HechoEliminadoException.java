package domain.exceptions;

public class HechoEliminadoException extends RuntimeException {
  public HechoEliminadoException() {
    super("El hecho al que intentas acceder ha sido eliminado.");
  }
}
