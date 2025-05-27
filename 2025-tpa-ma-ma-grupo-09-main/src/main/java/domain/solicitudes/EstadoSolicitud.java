package domain.solicitudes;

import domain.Administrador;
import lombok.Getter;

import java.time.LocalDateTime;

@Getter
public class EstadoSolicitud {
  private PosibleEstado posibleEstado;
  private Administrador administrador;
  private LocalDateTime fechaDeRevision;

  public EstadoSolicitud() {
    this.posibleEstado = PosibleEstado.PENDIENTE;
  }

  public EstadoSolicitud(PosibleEstado posibleEstado, Administrador administrador, LocalDateTime fechaDeRevision) {
    this.posibleEstado = posibleEstado;
    this.administrador = administrador;
    this.fechaDeRevision = fechaDeRevision;
  }
}
