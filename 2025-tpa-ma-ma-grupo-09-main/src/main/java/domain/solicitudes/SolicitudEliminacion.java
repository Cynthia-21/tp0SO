package domain.solicitudes;

import domain.Contribuyente;
import domain.Administrador;
import domain.Hecho;
import lombok.Getter;
import lombok.Setter;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

@Setter
@Getter
public class SolicitudEliminacion {
  private Contribuyente solicitante;
  private Hecho hechoAEliminar;
  private MotivoSolicitud motivoSolicitud;
  private LocalDateTime fechaSolicitud;
  private List<EstadoSolicitud> estadosDeSolicitud;


  public SolicitudEliminacion(Contribuyente solicitante, Hecho hechoAEliminar, MotivoSolicitud motivoSolicitud) {
    this.solicitante = solicitante;
    this.hechoAEliminar = hechoAEliminar;
    this.motivoSolicitud = motivoSolicitud;
    this.estadosDeSolicitud = new ArrayList<>();
    this.estadosDeSolicitud.add(new EstadoSolicitud());
    this.fechaSolicitud = LocalDateTime.now();
  }

  public void aceptarSolicitud(LocalDateTime fechaDeRevision, Administrador administrador) {
    this.hechoAEliminar.setEstaEliminado(true);
    this.estadosDeSolicitud.add(new EstadoSolicitud(PosibleEstado.CONFIRMADA, administrador, fechaDeRevision));
  }

  public void rechazarSolicitud(LocalDateTime fechaDeRevision, Administrador administrador) {
    this.hechoAEliminar.setEstaEliminado(false);
    this.estadosDeSolicitud.add(new EstadoSolicitud(PosibleEstado.RECHAZADA, administrador, fechaDeRevision));
  }

  public PosibleEstado getEstadoSolicitudActual() {
    return this.estadosDeSolicitud.get(this.estadosDeSolicitud.size() - 1).getPosibleEstado();
  }

  public LocalDateTime getFechaUltimaRevision() {
    if (this.estadosDeSolicitud.size() == 1) {
      throw new RuntimeException("Esta solicitud no tiene fechas de revisión aún.");
    }
    return this.getUltimoEstadoSolicitud().getFechaDeRevision();
  }

  public EstadoSolicitud getUltimoEstadoSolicitud() {
    return this.estadosDeSolicitud.get(this.estadosDeSolicitud.size() - 1);
  }
}
