package ar.edu.utn.frba.dds.servicioagregador.models.domain.criteriosPertenencia;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Categoria;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import lombok.Getter;

@Getter
public class CriterioCategoria implements ICriterioDePertenencia {
  private Categoria categoria;

  public CriterioCategoria(Categoria categoria) {
    this.categoria = categoria;
  }

  public Long getCategoriaId() {
    return categoria.getId();
  }

  @Override
  public Boolean cumpleCriterio(Hecho hecho) {
    return hecho.getCategorias().stream().anyMatch(categoria -> categoria.equals(this.categoria));
  }
}
