package domain.solicitudes;

import lombok.Getter;

@Getter
public class MotivoSolicitud {
  private String descripcion;

  public MotivoSolicitud(String descripcion) {
    checkearLargoMotivo(descripcion);
    this.descripcion = descripcion;
  }

  public void setDescripcion(String descripcion) {
    checkearLargoMotivo(descripcion);
    this.descripcion = descripcion;
  }

  private void checkearLargoMotivo(String motivo) {
    if (motivo.length() < 500) {
      throw new IllegalArgumentException("Motivo de solicitud debe tener 500 characters");
    }
  }
}
