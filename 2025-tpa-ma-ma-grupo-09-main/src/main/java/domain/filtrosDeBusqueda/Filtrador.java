package domain.filtrosDeBusqueda;

import domain.Coleccion;
import domain.Hecho;

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