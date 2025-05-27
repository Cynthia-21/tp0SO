package ar.edu.utn.frba.dds.fuentedinamica.models.domain.fuentes;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Hecho;

import java.util.List;

public interface Fuente {
  public List<Hecho> importarHechos();

}
