package ar.edu.utn.frba.dds.servicioagregador.models.domain.criteriosPertenencia;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.ubicacion.Ubicacion;
import lombok.Getter;

public class CriterioUbicacion implements ICriterioDePertenencia {
  private Ubicacion ubicacion;

  public CriterioUbicacion(Ubicacion ubicacion) {
    this.ubicacion = ubicacion;
  }

  public Double getLatitud() {
    return ubicacion.getLatitud();
  }

  public Double getLongitud() {
    return ubicacion.getLongitud();
  }

  @Override
  public Boolean cumpleCriterio(Hecho hecho) {
    return hecho.getUbicacion().esIgualA(this.ubicacion);
  }
}
