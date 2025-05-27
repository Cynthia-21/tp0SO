package ar.edu.utn.frba.dds.fuentedinamica.models.domain.solicitudes;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Administrador;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Contribuyente;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Hecho;
import lombok.Getter;
import lombok.Setter;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

@Setter
@Getter
public class SolicitudDeModificacion {
  private Long id;
  private Contribuyente solicitante;
  private Hecho hechoARevisar;
  private Hecho hechoModificado;
  private LocalDateTime fechaDeAlta;
  private List<EstadoRevision> estadosRevision;
  private List<Hecho> historialModificados; //guarda los hechos originales


  public SolicitudDeModificacion(Contribuyente solicitante, Hecho hechoARevisar, Hecho hechoModificado) {
    this.solicitante = solicitante;
    this.hechoARevisar = hechoARevisar;
    this.hechoModificado = hechoModificado;
    this.estadosRevision = new ArrayList<>();
    this.estadosRevision.add(new EstadoRevision());
    this.fechaDeAlta = LocalDateTime.now();
  }

  public void aceptarRevision(LocalDateTime fechaDeRevision, Administrador administrador) {
    this.historialModificados.add(this.hechoARevisar);
    this.hechoARevisar = this.hechoModificado;
    this.estadosRevision.add(new EstadoRevision(PosibleEstado.CONFIRMADA, administrador, fechaDeRevision));
  }

  public void aceptarConSugerencia(LocalDateTime fechaDeRevision, Administrador administrador, String comentario) {
    this.historialModificados.add(this.hechoARevisar);
    this.hechoARevisar = this.hechoModificado;
    this.estadosRevision.add(new EstadoRevision(PosibleEstado.CONFIRMADA_SUGERENCIA, administrador, fechaDeRevision));
  }

  public void rechazarRevision(LocalDateTime fechaDeRevision, Administrador administrador) {
    this.estadosRevision.add(new EstadoRevision(PosibleEstado.RECHAZADA, administrador, fechaDeRevision));
  }

  public PosibleEstado getEstadoRevisionActual() {
    return this.estadosRevision.get(this.estadosRevision.size() - 1).getEstadoRevision();
  }

  public LocalDateTime getFechaUltimaRevision() {
    if (this.estadosRevision.size() == 1) {
      throw new RuntimeException("Esta solicitud no tiene fechas de revisión aún.");
    }
    return this.getUltimoEstadoRevision().getFechaDeRevision();
  }

  public EstadoRevision getUltimoEstadoRevision() {
    return this.estadosRevision.get(this.estadosRevision.size() - 1);
  }
}
