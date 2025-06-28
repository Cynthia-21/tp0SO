package ar.edu.utn.frba.dds.fuenteestatica.services.impl;

import ar.edu.utn.frba.dds.fuenteestatica.models.domain.Hecho;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.dtos.output.HechoOutputDTO;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.Fuente;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.estrategiasFuenteEstatica.Archivo;
import ar.edu.utn.frba.dds.fuenteestatica.models.repositories.IHechoRepository;
import ar.edu.utn.frba.dds.fuenteestatica.services.IHechoService;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class HechoService implements IHechoService {
  @Autowired
  IHechoRepository repositorioDeHechos;
  @Override
  public List<Hecho> obtenerHechosDeFuente(Fuente unaFuente) {
    return unaFuente.importarHechos();
  }
  public List<Hecho> obtenerHechosAPartirDeArchivo(String nombrearchivo){
    return this.repositorioDeHechos.filtrarPorArchivo(nombrearchivo);
  }

  public List<Hecho> obtenerTodosHechos(){
    return repositorioDeHechos.findAll();
  }
}
