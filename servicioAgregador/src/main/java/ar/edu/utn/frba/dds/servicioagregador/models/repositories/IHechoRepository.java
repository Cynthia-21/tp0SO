package ar.edu.utn.frba.dds.servicioagregador.models.repositories;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import java.util.List;
import java.util.Optional;

public interface IHechoRepository {
  Hecho save (Hecho hecho);
  List<Hecho> saveVarios(List<Hecho> hechos);
  List<Hecho> findAll();
  Hecho findById(Long id);
}
