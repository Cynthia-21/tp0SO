package ar.edu.utn.frba.dds.servicioagregador.models.domain.criteriosPertenencia;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import lombok.Getter;

import java.time.LocalDate;
import java.time.LocalDateTime;

@Getter
public class CriterioFecha implements ICriterioDePertenencia {
  private LocalDateTime fechaInicial;
  private LocalDateTime fechaFinal;

  public CriterioFecha(LocalDateTime fechaInicial, LocalDateTime fechaFinal) {
    this.fechaInicial = fechaInicial;
    this.fechaFinal = fechaFinal;
  }

  @Override
  public Boolean cumpleCriterio(Hecho hecho) {
    return hecho.getFechaAcontecimiento().isAfter(this.fechaInicial)
        && hecho.getFechaAcontecimiento().isBefore(this.fechaFinal);
  }
}
