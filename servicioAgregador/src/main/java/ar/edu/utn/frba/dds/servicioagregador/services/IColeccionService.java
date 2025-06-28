package ar.edu.utn.frba.dds.servicioagregador.services;

import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.ColeccionInputDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.output.ColeccionOutputDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.output.HechoOutputDTO;
import reactor.core.publisher.Mono;

import java.util.List;
import java.util.stream.Collectors;

public interface IColeccionService {
  ColeccionOutputDTO crearColeccion(ColeccionInputDTO coleccionDTO);
  Mono<Void> refrescarColecciones();
  public ColeccionOutputDTO obtenerColeccionPorId(String id);
  public List<HechoOutputDTO> obtenerHechosDeColeccionPorId(String id);
}
