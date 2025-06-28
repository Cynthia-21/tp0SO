package ar.edu.utn.frba.dds.servicioagregador.services;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Coleccion;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.Fuente;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.output.HechoOutputDTO;

import java.util.List;

public interface IHechoService {
  List<HechoOutputDTO> obtenerTodosLosHechos();
  List<HechoOutputDTO> obtenerHechosDeFuente(Fuente fuente);
  List<Hecho> obtenerHechosRecientes(Fuente fuente);
}
