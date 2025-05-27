package domain.criteriosPertenencia;

import domain.Hecho;

import java.util.ArrayList;
import java.util.List;

public class CriterioDePertenencia {
  private List<ICriterioDePertenencia> criterios;

  public CriterioDePertenencia() {
    this.criterios = new ArrayList<>();
  }

  public void agregarCriterio(ICriterioDePertenencia criterio) {
    this.criterios.add(criterio);
  }

  public Boolean cumpleCriterios(Hecho hecho) {
    return criterios
        .stream()
        .allMatch(criterio -> criterio.cumpleCriterio(hecho));
  }
}
