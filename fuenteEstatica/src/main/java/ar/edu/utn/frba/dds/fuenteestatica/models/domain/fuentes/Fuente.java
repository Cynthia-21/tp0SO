package ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes;

import ar.edu.utn.frba.dds.fuenteestatica.models.domain.Hecho;

import java.util.List;

public interface Fuente {
  public List<Hecho> importarHechos();
}
