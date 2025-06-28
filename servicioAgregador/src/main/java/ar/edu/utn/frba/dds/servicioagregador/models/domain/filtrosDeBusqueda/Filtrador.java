package ar.edu.utn.frba.dds.servicioagregador.models.domain.filtrosDeBusqueda;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Coleccion;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;

import java.util.List;
import java.util.stream.Collectors;

public class Filtrador {

  public List<Hecho> filtrar(Coleccion coleccion, Filtro filtro) {
    return coleccion.getHechos()
        .stream()
        .filter(hecho -> filtro.cumpleFiltro(hecho))
        .collect(Collectors.toList());
  }
}