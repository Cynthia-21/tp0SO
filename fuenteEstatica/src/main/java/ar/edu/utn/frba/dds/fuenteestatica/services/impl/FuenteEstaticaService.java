package ar.edu.utn.frba.dds.fuenteestatica.services.impl;

import ar.edu.utn.frba.dds.fuenteestatica.models.repositories.IFuenteEstaticaRepository;
import ar.edu.utn.frba.dds.fuenteestatica.services.IFuenteEstaticaService;
import org.springframework.stereotype.Service;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.Fuente;

@Service
public class FuenteEstaticaService implements IFuenteEstaticaService {
  private IFuenteEstaticaRepository repositorioDeFuentes;
  public Fuente buscarPorID(Long id){
    return repositorioDeFuentes.findById(id);
  }
}
