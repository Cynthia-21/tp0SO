package ar.edu.utn.frba.dds.fuenteestatica.services;

import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.Fuente;
import ar.edu.utn.frba.dds.fuenteestatica.models.repositories.IFuenteEstaticaRepository;

public interface IFuenteEstaticaService {
  public Fuente buscarPorID(Long id);

}
