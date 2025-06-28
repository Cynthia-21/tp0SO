package ar.edu.utn.frba.dds.fuenteestatica.services;

import ar.edu.utn.frba.dds.fuenteestatica.models.domain.Hecho;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.dtos.output.HechoOutputDTO;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.Fuente;
import java.util.List;

public interface IHechoService {
  public List<Hecho> obtenerHechosDeFuente(Fuente unaFuente);
}
