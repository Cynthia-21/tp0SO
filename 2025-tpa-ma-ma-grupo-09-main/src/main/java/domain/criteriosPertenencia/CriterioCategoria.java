package domain.criteriosPertenencia;

import domain.Categoria;
import domain.Hecho;

public class CriterioCategoria implements ICriterioDePertenencia {
  private Categoria categoria;

  public CriterioCategoria(Categoria categoria) {
    this.categoria = categoria;
  }

  @Override
  public Boolean cumpleCriterio(Hecho hecho) {
    return hecho.getCategorias().stream().anyMatch(categoria -> categoria.equals(this.categoria));
  }
}
