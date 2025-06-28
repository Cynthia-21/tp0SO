package ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;

import java.util.List;

public interface Fuente {
  List<Hecho> importarHechos();

  List<Hecho> importarHechosRecientes();

  String getTipo();

  void eliminarHecho(Hecho hecho);
}
