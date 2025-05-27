package domain.criteriosPertenencia;

import domain.Hecho;
import java.time.LocalDate;

public class CriterioFecha implements ICriterioDePertenencia {
  private LocalDate fechaInicial;
  private LocalDate fechaFinal;

  public CriterioFecha(LocalDate fechaInicial, LocalDate fechaFinal) {
    this.fechaInicial = fechaInicial;
    this.fechaFinal = fechaFinal;
  }

  @Override
  public Boolean cumpleCriterio(Hecho hecho) {
    return hecho.getFechaAcontecimiento().isAfter(this.fechaInicial)
        && hecho.getFechaAcontecimiento().isBefore(this.fechaFinal);
  }
}
