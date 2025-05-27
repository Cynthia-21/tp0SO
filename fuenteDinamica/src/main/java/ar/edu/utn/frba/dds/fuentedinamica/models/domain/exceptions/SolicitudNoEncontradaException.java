package ar.edu.utn.frba.dds.fuentedinamica.models.domain.exceptions;

public class SolicitudNoEncontradaException extends RuntimeException {
  public SolicitudNoEncontradaException() {
    super("La solicitud no puede ser encontrada.");
  }
}
