package ar.edu.utn.frba.dds.servicioagregador.models.domain.criteriosPertenencia;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import lombok.Getter;
import lombok.Setter;

import java.util.ArrayList;
import java.util.List;

@Getter
public class CriterioDePertenencia {
  private List<ICriterioDePertenencia> criterios;
  @Setter
  private Boolean fueModificado;

  public CriterioDePertenencia() {
    this.criterios = new ArrayList<>();
    this.fueModificado = false;
  }

  public void agregarCriterio(ICriterioDePertenencia criterio) {
    this.criterios.add(criterio);
    this.fueModificado = true;
  }

  public void removerCriterio(ICriterioDePertenencia criterio) {
    this.criterios.remove(criterio);
    this.fueModificado = true;
  }

  public Boolean cumpleCriterios(Hecho hecho) {
    return this.criterios
        .stream()
        .allMatch(criterio -> criterio.cumpleCriterio(hecho));
  }
}
