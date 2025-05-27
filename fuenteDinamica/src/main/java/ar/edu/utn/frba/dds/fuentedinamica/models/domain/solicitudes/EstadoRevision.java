package ar.edu.utn.frba.dds.fuentedinamica.models.domain.solicitudes;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Administrador;
import lombok.Getter;

import java.time.LocalDateTime;

@Getter
public class EstadoRevision {
  private PosibleEstado estadoRevision;
  private Administrador quienReviso;
  private LocalDateTime fechaDeRevision;
  private String comentarioSugerencia;

  public EstadoRevision() {
    this.estadoRevision = PosibleEstado.PENDIENTE;
  }

  public EstadoRevision(PosibleEstado estadoRevision, Administrador quienReviso, LocalDateTime fechaDeRevision) {
    this.estadoRevision = estadoRevision;
    this.quienReviso = quienReviso;
    this.fechaDeRevision = fechaDeRevision;
  }
}
