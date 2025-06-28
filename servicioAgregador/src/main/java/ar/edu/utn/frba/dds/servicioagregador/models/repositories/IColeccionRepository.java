package ar.edu.utn.frba.dds.servicioagregador.models.repositories;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Coleccion;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.Fuente;

import java.util.List;
import java.util.Optional;

public interface IColeccionRepository {
  Coleccion save(Coleccion coleccion);
  List<Coleccion> findAll();
  Coleccion findById(String id);
  List<Coleccion> findByFuente(Fuente fuente);
  List<Coleccion> findByCambioCriterio();
  void delete(Coleccion coleccion);
}
