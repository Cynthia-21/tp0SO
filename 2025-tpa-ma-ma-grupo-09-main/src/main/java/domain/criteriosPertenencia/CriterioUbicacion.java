package domain.criteriosPertenencia;

import domain.Hecho;
import domain.ubicacion.Ubicacion;

public class CriterioUbicacion implements ICriterioDePertenencia {
  private Ubicacion ubicacion;

  public CriterioUbicacion(Ubicacion ubicacion) {
    this.ubicacion = ubicacion;
  }

  @Override
  public Boolean cumpleCriterio(Hecho hecho) {
    return hecho.getUbicacion().esIgualA(this.ubicacion);
  }
}
