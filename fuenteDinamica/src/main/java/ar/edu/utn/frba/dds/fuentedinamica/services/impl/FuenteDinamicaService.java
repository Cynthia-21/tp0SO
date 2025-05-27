package ar.edu.utn.frba.dds.fuentedinamica.services.impl;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.fuentes.FuenteDinamica;
import ar.edu.utn.frba.dds.fuentedinamica.models.repositories.IFuenteDinamicaRepository;
import ar.edu.utn.frba.dds.fuentedinamica.services.IFuenteDinamicaService;
import org.springframework.stereotype.Service;

@Service
public class FuenteDinamicaService implements IFuenteDinamicaService {
  private IFuenteDinamicaRepository fuenteDinamicaRepository;
  public FuenteDinamica buscarPorID(Long id){
    return fuenteDinamicaRepository.findById(id);
  }
}
