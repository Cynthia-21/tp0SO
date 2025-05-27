package ar.edu.utn.frba.dds.fuentedinamica.services;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.fuentes.FuenteDinamica;

public interface IFuenteDinamicaService {
  public FuenteDinamica buscarPorID(Long id);
}